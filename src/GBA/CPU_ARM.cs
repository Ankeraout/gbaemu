namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ARMOpcodeHandler(CPU cpu, uint opcode);

        private static ARMOpcodeHandler[] armOpcodeHandlerTable;

        private uint shifterResult;
        private bool shifterCarry;

        private static void InitARMDecodeTable() {
            armOpcodeHandlerTable = new ARMOpcodeHandler[4096];

            for(int i = 0; i < 4096; i++) {
                if((i & 0xf00) == 0xf00) {
                    armOpcodeHandlerTable[i] = OpcodeArmSwi;
                } else if(i == 0x121) {
                    armOpcodeHandlerTable[i] = OpcodeArmBx;
                } else if((i & 0xfbf) == 0x100) {
                    armOpcodeHandlerTable[i] = OpcodeArmPsrTransfer;
                } else if((i & 0xfbf) == 0x120) {
                    armOpcodeHandlerTable[i] = OpcodeArmPsrTransfer;
                } else if((i & 0xdb0) == 0x120) {
                    armOpcodeHandlerTable[i] = OpcodeArmPsrTransfer;
                } else if((i & 0xc00) == 0x000) {
                    switch((i & 0x1e0) >> 5) {
                        case 0b0000:
                            armOpcodeHandlerTable[i] = OpcodeArmAnd;
                            break;

                        case 0b0001:
                            armOpcodeHandlerTable[i] = OpcodeArmEor;
                            break;

                        case 0b0010:
                            armOpcodeHandlerTable[i] = OpcodeArmSub;
                            break;

                        case 0b0011:
                            armOpcodeHandlerTable[i] = OpcodeArmRsb;
                            break;

                        case 0b0100:
                            armOpcodeHandlerTable[i] = OpcodeArmAdd;
                            break;

                        case 0b0101:
                            armOpcodeHandlerTable[i] = OpcodeArmAdc;
                            break;

                        case 0b0110:
                            armOpcodeHandlerTable[i] = OpcodeArmSbc;
                            break;
                        
                        case 0b0111:
                            armOpcodeHandlerTable[i] = OpcodeArmRsc;
                            break;

                        case 0b1000:
                            armOpcodeHandlerTable[i] = OpcodeArmTst;
                            break;

                        case 0b1001:
                            armOpcodeHandlerTable[i] = OpcodeArmTeq;
                            break;
                        
                        case 0b1010:
                            armOpcodeHandlerTable[i] = OpcodeArmCmp;
                            break;

                        case 0b1011:
                            armOpcodeHandlerTable[i] = OpcodeArmCmn;
                            break;

                        case 0b1100:
                            armOpcodeHandlerTable[i] = OpcodeArmOrr;
                            break;

                        case 0b1101:
                            armOpcodeHandlerTable[i] = OpcodeArmMov;
                            break;

                        case 0b1110:
                            armOpcodeHandlerTable[i] = OpcodeArmBic;
                            break;

                        case 0b1111:
                            armOpcodeHandlerTable[i] = OpcodeArmMvn;
                            break;
                    }
                } else if(((i & 0xc00) == 0x400) & ((i & 0x201) != 0x201)) {
                    armOpcodeHandlerTable[i] = OpcodeArmSingleDataTransfer;
                } else if((i & 0xe00) == 0xa00) {
                    armOpcodeHandlerTable[i] = OpcodeArmB;
                }
            }
        }

        private ARMOpcodeHandler DecodeARMOpcode(uint opcode) {
            return armOpcodeHandlerTable[((opcode >> 16) & 0xff0) | ((opcode >> 4) & 0xf)];
        }

        private static void OpcodeArmBx(CPU cpu, uint opcode) {
            if((opcode & 0x0ffffff0) != 0x012fff10) {
                cpu.RaiseUND();
            } else {
                uint dest = cpu.r[opcode & 0x0000000f];

                cpu.flagT = BitUtils.BitTest32(dest, 0);
                cpu.PerformJump(dest);
            }
        }

        private static void OpcodeArmSwi(CPU cpu, uint opcode) {
            cpu.RaiseSWI();
        }

        private static void OpcodeArmSingleDataTransfer(CPU cpu, uint opcode) {
            bool i = BitUtils.BitTest32(opcode, 25);
            bool p = BitUtils.BitTest32(opcode, 24);
            bool u = BitUtils.BitTest32(opcode, 23);
            bool b = BitUtils.BitTest32(opcode, 22);
            bool w = BitUtils.BitTest32(opcode, 21);
            bool l = BitUtils.BitTest32(opcode, 20);
            uint rn = (opcode & 0x000f0000) >> 16;
            uint rd = (opcode & 0x0000f000) >> 12;
            uint rn_v = cpu.r[rn];
            uint rd_v = cpu.r[rd];

            if(i && rn == 15) {
                rn_v += 4;
            }

            if(i && rd == 15) {
                rd_v += 4;
            }

            if(i) {
                cpu.OpcodeArmDataProcessingShifter(opcode ^ 0x02000000);
            } else {
                cpu.shifterResult = opcode & 0x00000fff;
            }

            uint offset = rn_v;

            if(p) {
                if(u) {
                    offset += cpu.shifterResult;
                } else {
                    offset -= cpu.shifterResult;
                }
            } else {
                w = true;
            }

            if(w) {
                if(u) {
                    if(p) {
                        cpu.OpcodeArmDataProcessingWriteRegister(rn, offset);
                    } else {
                        cpu.OpcodeArmDataProcessingWriteRegister(rn, offset + cpu.shifterResult);
                    }
                } else {
                    if(p) {
                        cpu.OpcodeArmDataProcessingWriteRegister(rn, offset);
                    } else {
                        cpu.OpcodeArmDataProcessingWriteRegister(rn, offset - cpu.shifterResult);
                    }
                }
            }

            if(l) {
                if(b) {
                    cpu.r[rd] = cpu.gba.Bus.Read8(offset);
                } else {
                    cpu.r[rd] = BitUtils.RotateRight32(cpu.gba.Bus.Read32(offset), (int)((offset & 0x00000003) << 3));
                }
            } else {
                if(b) {
                    cpu.gba.Bus.Write8(offset, (byte)rd_v);
                } else {
                    cpu.gba.Bus.Write32(offset, rd_v);
                }
            }
        }

        private static void OpcodeArmPsrTransfer(CPU cpu, uint opcode) {
            bool msr = BitUtils.BitTest32(opcode, 21);
            bool spsr = BitUtils.BitTest32(opcode, 22);

            if(msr) {
                bool disableProtection = BitUtils.BitTest32(opcode, 16);

                if(disableProtection) {
                    uint rm = opcode & 0x0000000f;

                    if(spsr) {
                        cpu.Spsr = cpu.r[rm];
                    } else {
                        cpu.Cpsr = cpu.r[rm];
                    }
                } else {
                    bool immediate = BitUtils.BitTest32(opcode, 25);

                    uint operand;

                    if(immediate) {
                        operand = BitUtils.RotateRight32(opcode & 0x000000ff, (int)(opcode & 0x00000f00) >> 7);
                    } else {
                        uint rm = opcode & 0x0000000f;
                        operand = cpu.r[rm];
                    }

                    if(spsr) {
                        cpu.Spsr = operand | (cpu.Spsr & 0xf0000000);
                    } else {
                        cpu.Cpsr = operand | (cpu.Cpsr & 0xf0000000);
                    }
                }
            } else {
                uint rd = (opcode & 0x0000f000) >> 12;

                if(spsr) {
                    cpu.r[rd] = cpu.Spsr;
                } else {
                    cpu.r[rd] = cpu.Cpsr;
                }
            }
        }

        private void OpcodeArmDataProcessingWriteRegister(uint rd, uint value) {
            if(rd == 15) {
                PerformJump(value);
            } else {
                r[rd] = value;
            }
        }

        private void OpcodeArmDataProcessingLogicalSetFlags(uint result) {
            flagN = BitUtils.BitTest32(result, 31);
            flagZ = result == 0;
            flagC = shifterCarry;
        }

        private void OpcodeArmDataProcessingArithmeticalSetFlags(uint result) {
            flagN = BitUtils.BitTest32(result, 31);
            flagZ = result == 0;
        }

        private void OpcodeArmDataProcessingShifter(uint opcode) {
            // Test I bit
            if((opcode & 0x02000000) == 0) {
                // Shift using a register
                uint rm = opcode & 0x0000000f;
                uint rm_v = r[rm];

                if(rm == 15) {
                    rm_v += 4;
                }

                if((opcode & 0x00000010) == 0) {
                    int immediate = (int)((opcode & 0x00000f80) >> 7);

                    // Shift the register value with an immediate value
                    switch((opcode & 0x060) >> 5) {
                        case 0b00: // LSL
                            if(immediate == 0) {
                                shifterResult = rm_v;
                                shifterCarry = flagC;
                            } else {
                                shifterResult = rm_v << immediate;
                                shifterCarry = BitUtils.BitTest32(rm_v >> (32 - immediate), 0);
                            }

                            break;

                        case 0b01: // LSR
                            if(immediate == 0) {
                                shifterResult = 0;
                                shifterCarry = BitUtils.BitTest32(rm_v, 31);
                            } else {
                                shifterResult = rm_v >> immediate;
                                shifterCarry = BitUtils.BitTest32(rm_v >> (immediate - 1), 0);
                            }

                            break;

                        case 0b10: // ASR
                            if(immediate == 0) {
                                shifterResult = (uint)((int)rm_v >> 31);
                                shifterCarry = BitUtils.BitTest32(rm_v, 31);
                            } else {
                                shifterResult = (uint)((int)rm_v >> immediate);
                                shifterCarry = BitUtils.BitTest32(rm_v >> (immediate - 1), 0);
                            }
                            
                            break;

                        case 0b11: // ROR/RRX
                            if(immediate == 0) {
                                // RRX
                                shifterResult = (flagC ? 1U << 31 : 0) | (rm_v >> 1);
                                shifterCarry = BitUtils.BitTest32(rm_v, 0);
                            } else {
                                // ROR
                                shifterResult = BitUtils.RotateRight32(rm_v, immediate);
                                shifterCarry = BitUtils.BitTest32(rm_v >> (immediate - 1), 0);
                            }
                            
                            break;
                    }
                } else {
                    // Shift the register value with a register value
                    if((opcode & 0x00000080) != 0) {
                        throw new InvalidOpcodeException();
                    }

                    uint rs = (opcode & 0x00000f00) >> 8;
                    uint shift = r[rs];

                    if(rs == 15) {
                        shift += 4;
                    }

                    switch((opcode & 0x060) >> 5) {
                        case 0b00: // LSL
                            if(shift == 0) {
                                shifterResult = rm_v;
                                shifterCarry = flagC;
                            } else if(shift < 32) {
                                shifterResult = rm_v << (int)shift;
                                shifterCarry = BitUtils.BitTest32(rm_v >> (int)(shift - 1), 1);
                            } else if(shift == 32) {
                                shifterResult = 0;
                                shifterCarry = BitUtils.BitTest32(rm_v, 0);
                            } else {
                                shifterResult = 0;
                                shifterCarry = false;
                            }

                            break;

                        case 0b01: // LSR
                            if(shift == 0) {
                                shifterResult = rm_v;
                                shifterCarry = flagC;
                            } else if(shift < 32) {
                                shifterResult = rm_v >> (int)shift;
                                shifterCarry = BitUtils.BitTest32(rm_v >> (int)(shift - 1), 0);
                            } else if(shift == 32) {
                                shifterResult = 0;
                                shifterCarry = BitUtils.BitTest32(rm_v, 31);
                            } else {
                                shifterResult = 0;
                                shifterCarry = false;
                            }

                            break;

                        case 0b10: // ASR
                            if(shift == 0) {
                                shifterResult = rm_v;
                                shifterCarry = flagC;
                            } else if(shift < 32) {
                                shifterResult = (uint)((int)rm_v >> (int)shift);
                                shifterCarry = BitUtils.BitTest32(rm_v >> (int)(shift - 1), 0);
                            } else {
                                shifterResult = (uint)((int)rm_v >> 31);
                                shifterCarry = BitUtils.BitTest32(rm_v, 31);
                            }

                            break;

                        case 0b11: // ROR
                            int rotation = (int)shift & 0x1f;

                            if(shift == 0) {
                                shifterResult = rm_v;
                                shifterCarry = flagC;
                            } else if(rotation == 0) {
                                shifterResult = rm_v;
                                shifterCarry = BitUtils.BitTest32(rm_v, 31);
                            } else {
                                shifterResult = BitUtils.RotateRight32(rm_v, rotation);
                                shifterCarry = BitUtils.BitTest32(rm_v >> (rotation - 1), 0);
                            }

                            break;
                    }
                }
            } else {
                // Shift using an immediate value
                uint rotation = (opcode >> 7) & 0x1e;
                uint immediate = opcode & 0xff;

                if(rotation == 0) {
                    shifterResult = immediate;
                    shifterCarry = flagC;
                } else {
                    shifterResult = BitUtils.RotateRight32(immediate, (int)rotation);
                    shifterCarry = BitUtils.BitTest32(shifterResult, 31);
                }
            }
        }

        private static void OpcodeArmAnd(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = rn_v & cpu.shifterResult;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmEor(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = rn_v ^ cpu.shifterResult;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmSub(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = rn_v - cpu.shifterResult;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.flagC = rn_v >= cpu.shifterResult;
                    cpu.flagV = result <= rn_v;
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmRsb(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = cpu.shifterResult - rn_v;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.flagC = cpu.shifterResult >= rn_v;
                    cpu.flagV = rn_v <= result;
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmAdd(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = rn_v + cpu.shifterResult;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.flagC = result < rn_v;
                    cpu.flagV = result < rn_v;
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmAdc(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            long result = rn_v + cpu.shifterResult + (cpu.flagC ? 1 : 0);

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.flagC = result > uint.MaxValue;
                    cpu.flagV = result > uint.MaxValue;
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags((uint)result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, (uint)result);
        }

        private static void OpcodeArmSbc(CPU cpu, uint opcode) {
            throw new System.NotImplementedException();
        }

        private static void OpcodeArmRsc(CPU cpu, uint opcode) {
            throw new System.NotImplementedException();
        }

        private static void OpcodeArmTst(CPU cpu, uint opcode) {
            bool s = BitUtils.BitTest32(opcode, 20);

            if(!s) {
                throw new InvalidOpcodeException();
            }

            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;

            uint result = rn_v & cpu.shifterResult;

            if(rd == 15) {
                cpu.Cpsr = cpu.Spsr;
            } else {
                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
            }
        }

        private static void OpcodeArmTeq(CPU cpu, uint opcode) {
            bool s = BitUtils.BitTest32(opcode, 20);

            if(!s) {
                throw new InvalidOpcodeException();
            }

            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;

            uint result = rn_v ^ cpu.shifterResult;

            if(rd == 15) {
                cpu.Cpsr = cpu.Spsr;
            } else {
                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
            }
        }

        private static void OpcodeArmCmp(CPU cpu, uint opcode) {
            bool s = BitUtils.BitTest32(opcode, 20);

            if(!s) {
                throw new InvalidOpcodeException();
            }

            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;

            uint result = rn_v - cpu.shifterResult;

            if(rd == 15) {
                cpu.Cpsr = cpu.Spsr;
            } else {
                cpu.flagC = rn_v >= cpu.shifterResult;
                cpu.flagV = result <= rn_v;
                cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
            }
        }

        private static void OpcodeArmCmn(CPU cpu, uint opcode) {
            bool s = BitUtils.BitTest32(opcode, 20);

            if(!s) {
                throw new InvalidOpcodeException();
            }

            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;

            uint result = rn_v + cpu.shifterResult;

            if(rd == 15) {
                cpu.Cpsr = cpu.Spsr;
            } else {
                cpu.flagC = result < rn_v;
                cpu.flagV = result < rn_v;
                cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
            }
        }

        private static void OpcodeArmOrr(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = rn_v | cpu.shifterResult;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmMov(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.OpcodeArmDataProcessingLogicalSetFlags(cpu.shifterResult);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, cpu.shifterResult);
        }

        private static void OpcodeArmBic(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = rn_v & ~cpu.shifterResult;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmMvn(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(BitUtils.BitTest32(opcode, 25)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = ~cpu.shifterResult;

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static void OpcodeArmB(CPU cpu, uint opcode) {
            bool l = BitUtils.BitTest32(opcode, 24);

            if(l) {
                cpu.r[14] = cpu.r[15] - 4;
            }
            
            cpu.PerformJump(cpu.r[15] + ((opcode & 0x00ffffff) << 2));
        }
    }
}
