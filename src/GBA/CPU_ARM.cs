namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ARMOpcodeHandler(CPU cpu, uint opcode);

        private static ARMOpcodeHandler[] armOpcodeHandlerTable;

        private uint shifterResult;
        private bool shifterCarry;

        private static void InitARMDecodeTable() {
            armOpcodeHandlerTable = new ARMOpcodeHandler[4096];

            for(int i = 0; i < 4096; i++) {
                if((i & 0xc00) == 0x000) {
                    switch((i & 0x1e0) >> 5) {
                        case 0b0000:
                            armOpcodeHandlerTable[i] = OpcodeArmAnd;
                            break;

                        case 0b0001:
                            armOpcodeHandlerTable[i] = OpcodeArmEor;
                            break;

                        case 0b1000:
                            armOpcodeHandlerTable[i] = OpcodeArmTst;
                            break;

                        case 0b1001:
                            armOpcodeHandlerTable[i] = OpcodeArmTeq;
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
                } else if((i & 0xe00) == 0xa00) {
                    armOpcodeHandlerTable[i] = OpcodeArmB;
                }
            }
        }

        private ARMOpcodeHandler DecodeARMOpcode(uint opcode) {
            return armOpcodeHandlerTable[((opcode >> 16) & 0xff0) | ((opcode >> 4) & 0xf)];
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
                                shifterResult = rm_v >> (int)shift;
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
                }

                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
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
                }

                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
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
            }

            cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
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
            }

            cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
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
                }

                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
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
                }

                cpu.OpcodeArmDataProcessingLogicalSetFlags(cpu.shifterResult);
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
                }

                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
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
                }

                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
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
