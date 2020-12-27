namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ARMOpcodeHandler(CPU cpu, uint opcode);

        private static ARMOpcodeHandler[] armOpcodeHandlerTable;

        private uint shifterResult;
        private bool shifterCarry;

        private static void InitARMDecodeTable() {
            armOpcodeHandlerTable = new ARMOpcodeHandler[4096];

            for(int i = 0; i < 4096; i++) {
                uint opcode = (uint)(((i & 0xff0) << 16) | ((i & 0x00f) << 4));

                switch((opcode & 0x0c000000) >> 26) {
                    case 0b00:
                    if((opcode & 0x0ff000f0) == 0x01200010) {
                        armOpcodeHandlerTable[i] = OpcodeArmBx;
                    } else if((opcode & 0x02000090) == 0x00000090) {
                        if((opcode & 0x0fb000f0) == 0x01000090) {
                            armOpcodeHandlerTable[i] = OpcodeArmSwp;
                        } else if((opcode & 0x0fc000f0) == 0x00000090) {
                            armOpcodeHandlerTable[i] = OpcodeArmMul;
                        } else if((opcode & 0x0f8000f0) == 0x00800090) {
                            armOpcodeHandlerTable[i] = OpcodeArmMull;
                        } else if((opcode & 0x0e000090) == 0x00000090) {
                            armOpcodeHandlerTable[i] = OpcodeArmHalfwordSignedDataTransfer;
                        }
                    } else if((opcode & 0x01900000) == 0x01000000) {
                        if(BitUtils.BitTest32(opcode, 25) || (opcode & 0x000000f0) == 0x00000000) {
                            armOpcodeHandlerTable[i] = OpcodeArmPsrTransfer;
                        }
                    } else {
                        switch((opcode & 0x01e00000) >> 21) {
                            case 0b0000: armOpcodeHandlerTable[i] = OpcodeArmAnd; break;
                            case 0b0001: armOpcodeHandlerTable[i] = OpcodeArmEor; break;
                            case 0b0010: armOpcodeHandlerTable[i] = OpcodeArmSub; break;
                            case 0b0011: armOpcodeHandlerTable[i] = OpcodeArmRsb; break;
                            case 0b0100: armOpcodeHandlerTable[i] = OpcodeArmAdd; break;
                            case 0b0101: armOpcodeHandlerTable[i] = OpcodeArmAdc; break;
                            case 0b0110: armOpcodeHandlerTable[i] = OpcodeArmSbc; break;
                            case 0b0111: armOpcodeHandlerTable[i] = OpcodeArmRsc; break;
                            case 0b1000: armOpcodeHandlerTable[i] = OpcodeArmTst; break;
                            case 0b1001: armOpcodeHandlerTable[i] = OpcodeArmTeq; break;
                            case 0b1010: armOpcodeHandlerTable[i] = OpcodeArmCmp; break;
                            case 0b1011: armOpcodeHandlerTable[i] = OpcodeArmCmn; break;
                            case 0b1100: armOpcodeHandlerTable[i] = OpcodeArmOrr; break;
                            case 0b1101: armOpcodeHandlerTable[i] = OpcodeArmMov; break;
                            case 0b1110: armOpcodeHandlerTable[i] = OpcodeArmBic; break;
                            case 0b1111: armOpcodeHandlerTable[i] = OpcodeArmMvn; break;
                        }
                    }

                    break;

                    case 0b01:
                    if((opcode & 0x02000010) != 0x02000010) {
                        armOpcodeHandlerTable[i] = OpcodeArmSingleDataTransfer;
                    }

                    break;
                    
                    case 0b10:
                    if(BitUtils.BitTest32(opcode, 25)) {
                        armOpcodeHandlerTable[i] = OpcodeArmB;
                    } else {
                        armOpcodeHandlerTable[i] = OpcodeArmBlockDataTransfer;
                    }

                    break;
                    
                    case 0b11:
                    if((opcode & 0x03000000) == 0x03000000) {
                        armOpcodeHandlerTable[i] = OpcodeArmSwi;
                    }

                    break;
                }
            }
        }

        private ARMOpcodeHandler DecodeARMOpcode(uint opcode) {
            return armOpcodeHandlerTable[((opcode >> 16) & 0xff0) | ((opcode >> 4) & 0xf)];
        }

        private static void OpcodeArmHalfwordSignedDataTransfer(CPU cpu, uint opcode) {
            bool p = BitUtils.BitTest32(opcode, 24);
            bool u = BitUtils.BitTest32(opcode, 23);
            bool i = BitUtils.BitTest32(opcode, 22);
            bool w = BitUtils.BitTest32(opcode, 21);
            bool l = BitUtils.BitTest32(opcode, 20);
            bool s = BitUtils.BitTest32(opcode, 6);
            bool h = BitUtils.BitTest32(opcode, 5);
            uint rn = (opcode & 0x000f0000) >> 16;
            uint rd = (opcode & 0x0000f000) >> 12;

            if(!s && !h) {
                throw new InvalidOpcodeException();
            }

            uint addr = cpu.r[rn];
            uint offset;

            if(i) {
                offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
            } else {
                if(((opcode & 0x00000f00) >> 8) != 0) {
                    throw new InvalidOpcodeException();
                }

                uint rm = opcode & 0x0000000f;

                offset = cpu.r[rm];
            }
                
            if(p) {
                if(u) {
                    addr += offset;
                } else {
                    addr -= offset;
                }
            } else {
                w = true;
            }

            if(l) {
                if(w) {
                    if(p) {
                        cpu.r[rn] = addr;
                    } else {
                        if(u) {
                            cpu.r[rn] = addr + offset;
                        } else {
                            cpu.r[rn] = addr - offset;
                        }
                    }
                }

                if(!s && h) { // LDRH
                    if(BitUtils.BitTest32(addr, 0)) {
                        cpu.OpcodeArmDataProcessingWriteRegister(rd, BitUtils.RotateRight32(cpu.gba.Bus.Read16(addr & 0xfffffffe), 8));
                    } else {
                        cpu.OpcodeArmDataProcessingWriteRegister(rd, cpu.gba.Bus.Read16(addr & 0xfffffffe));
                    }
                } else if(!h || BitUtils.BitTest32(addr, 0)) { // LDRSB
                    uint readValue = cpu.gba.Bus.Read8(addr);

                    if(BitUtils.BitTest32(readValue, 7)) {
                        readValue |= 0xffffff00;
                    }

                    cpu.OpcodeArmDataProcessingWriteRegister(rd, readValue);
                } else { // LDRSH
                    uint readValue = cpu.gba.Bus.Read16(addr & 0xfffffffe);

                    if(BitUtils.BitTest32(readValue, 15)) {
                        readValue |= 0xffff0000;
                    }
                    
                    cpu.OpcodeArmDataProcessingWriteRegister(rd, readValue);
                }
            } else {
                uint rd_v = cpu.r[rd];

                if(rd == 15) {
                    rd_v += 4;
                }

                if(s) {
                    throw new InvalidOpcodeException();
                } else { // STRH
                    cpu.gba.Bus.Write16(addr, (ushort)rd_v);
                }

                if(w) {
                    if(p) {
                        cpu.r[rn] = addr;
                    } else {
                        if(u) {
                            cpu.r[rn] = addr + offset;
                        } else {
                            cpu.r[rn] = addr - offset;
                        }
                    }
                }
            }
        }

        private static void OpcodeArmBlockDataTransfer(CPU cpu, uint opcode) {
            bool p = BitUtils.BitTest32(opcode, 24);
            bool u = BitUtils.BitTest32(opcode, 23);
            bool s = BitUtils.BitTest32(opcode, 22);
            bool w = BitUtils.BitTest32(opcode, 21);
            bool l = BitUtils.BitTest32(opcode, 20);
            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];
            uint registerCount = BitUtils.HammingWeight16((ushort)opcode);
            ushort rlist = (ushort)opcode;

            uint addr = rn_v;

            if(rlist == 0) {
                registerCount = 16;
            }

            if(p) {
                if(u) {
                    addr += 4;
                } else {
                    addr -= 4 * registerCount;
                }
            } else {
                if(!u) {
                    addr -= 4 * (registerCount - 1);
                }
            }

            if(rlist == 0) {
                if(l) {
                    cpu.PerformJump(cpu.gba.Bus.Read32(addr));
                } else {
                    cpu.gba.Bus.Write32(addr, cpu.r[15] + 4);
                }

                if(u) {
                    cpu.r[rn] += 0x40;
                } else {
                    cpu.r[rn] -= 0x40;
                }
            } else {
                if(l) { // LDM
                    if(w) {
                        if(u) {
                            cpu.r[rn] = rn_v + 4 * registerCount;
                        } else {
                            cpu.r[rn] = rn_v - 4 * registerCount;
                        }
                    }

                    for(int i = 0; i < 16; i++) {
                        if(BitUtils.BitTest32(opcode, i)) {
                            if(s) {
                                bool r15 = i == 15;
                                bool notUserMode = cpu.mode != Mode.USR && cpu.mode != Mode.USR_OLD && cpu.mode != Mode.SYS;
                                bool registerBanked = ((cpu.mode == Mode.FIQ || cpu.mode == Mode.FIQ_OLD) && i >= 8) || i >= 13;

                                if(r15) {
                                    cpu.Cpsr = cpu.Spsr;
                                    cpu.PerformJump(cpu.gba.Bus.Read32(addr));
                                } else if(notUserMode && registerBanked) {
                                    cpu.r_usr[i - 8] = cpu.gba.Bus.Read32(addr);
                                } else{
                                    cpu.r[i] = cpu.gba.Bus.Read32(addr);
                                }
                            } else {
                                cpu.OpcodeArmDataProcessingWriteRegister((uint)i, cpu.gba.Bus.Read32(addr));
                            }

                            addr += 4;
                        }
                    }
                } else { // STM
                    bool firstCycle = true;
                    bool secondCycle = false;

                    for(int i = 0; i < 16; i++) {
                        if(BitUtils.BitTest16(rlist, i)) {
                            if(firstCycle) {
                                firstCycle = false;
                                secondCycle = true;
                            } else if(secondCycle) {
                                if(w) {
                                    if(u) {
                                        cpu.r[rn] = rn_v + registerCount * 4;
                                    } else {
                                        cpu.r[rn] = rn_v - registerCount * 4;
                                    }
                                }

                                secondCycle = false;
                            }

                            if(s) {
                                bool notUserMode = cpu.mode != Mode.USR && cpu.mode != Mode.USR_OLD && cpu.mode != Mode.SYS;
                                bool registerBanked = ((cpu.mode == Mode.FIQ || cpu.mode == Mode.FIQ_OLD) && i >= 8) || i == 13 || i == 14;

                                if(notUserMode && registerBanked) {
                                    cpu.gba.Bus.Write32(addr, cpu.r_usr[i - 8]);
                                } else{
                                    cpu.gba.Bus.Write32(addr, cpu.r[i]);
                                }
                            } else {
                                if(i == 15 && !secondCycle) {
                                    cpu.gba.Bus.Write32(addr, cpu.r[15] + 4);
                                } else {
                                    cpu.gba.Bus.Write32(addr, cpu.r[i]);
                                }
                            }

                            addr += 4;
                        }
                    }

                    if(w && secondCycle) {
                        if(u) {
                            cpu.r[rn] = rn_v + 4;
                        } else {
                            cpu.r[rn] = rn_v - 4;
                        }
                    }
                }
            }
        }

        private static void OpcodeArmSwp(CPU cpu, uint opcode) {
            bool b = BitUtils.BitTest32(opcode, 22);
            uint rn = (opcode & 0x000f0000) >> 16;
            uint rd = (opcode & 0x0000f000) >> 12;
            uint rm = opcode & 0x0000000f;

            uint rn_v = cpu.r[rn];

            if(b) {
                byte tmp = cpu.gba.Bus.Read8(rn_v);
                cpu.gba.Bus.Write8(rn_v, (byte)cpu.r[rm]);
                cpu.r[rd] = tmp;
            } else {
                uint tmp = BitUtils.RotateRight32(cpu.gba.Bus.Read32(rn_v), (int)(rn_v & 3) << 3);
                cpu.gba.Bus.Write32(rn_v, cpu.r[rm]);
                cpu.r[rd] = tmp;
            }
        }

        private static void OpcodeArmMul(CPU cpu, uint opcode) {
            bool a = BitUtils.BitTest32(opcode, 21);
            bool s = BitUtils.BitTest32(opcode, 20);
            uint rd = (opcode & 0x000f0000) >> 16;
            uint rs = (opcode & 0x00000f00) >> 8;
            uint rm = opcode & 0x0000000f;

            uint result = cpu.r[rm] * cpu.r[rs];
            
            if(a) {
                uint rn = (opcode & 0x0000f000) >> 12;
                result += cpu.r[rn];
            }

            cpu.r[rd] = result;

            if(s) {
                cpu.OpcodeArmDataProcessingLogicalSetFlags(result);
            }
        }

        private static void OpcodeArmMull(CPU cpu, uint opcode) {
            bool u = BitUtils.BitTest32(opcode, 22);
            bool a = BitUtils.BitTest32(opcode, 21);
            bool s = BitUtils.BitTest32(opcode, 20);
            uint rdhi = (opcode & 0x000f0000) >> 16;
            uint rdlo = (opcode & 0x0000f000) >> 12;
            uint rs = (opcode & 0x00000f00) >> 8;
            uint rm = opcode & 0x0000000f;

            ulong result;

            if(!u) {
                ulong rm_v = cpu.r[rm];
                ulong rs_v = cpu.r[rs];

                result = rm_v * rs_v;
            } else {
                long rm_v = (int)cpu.r[rm];
                long rs_v = (int)cpu.r[rs];
                
                result = (ulong)(rm_v * rs_v);
            }

            if(a) {
                result += ((ulong)cpu.r[rdhi] << 32) | cpu.r[rdlo];
            }

            cpu.r[rdhi] = (uint)(result >> 32);
            cpu.r[rdlo] = (uint)result;

            if(s) {
                cpu.flagZ = result == 0;
                cpu.flagN = BitUtils.BitTest64(result, 63);
            }
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

            if(!l && rd == 15) {
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
                    cpu.OpcodeArmDataProcessingWriteRegister(rd, cpu.gba.Bus.Read8(offset));
                } else {
                    cpu.OpcodeArmDataProcessingWriteRegister(rd, BitUtils.RotateRight32(cpu.gba.Bus.Read32(offset), (int)((offset & 0x00000003) << 3)));
                }
            } else {
                if(b) {
                    cpu.gba.Bus.Write8(offset, (byte)rd_v);
                } else {
                    cpu.gba.Bus.Write32(offset, rd_v);
                }
            }
        }

        private static void OpcodeArmPsrTransfer_Msr(CPU cpu, uint opcode, bool spsr, uint operand) {
            uint mask = 0;

            if(BitUtils.BitTest32(opcode, 19)) mask |= 0xff000000;
            if(BitUtils.BitTest32(opcode, 18)) mask |= 0x00ff0000;
            if(BitUtils.BitTest32(opcode, 17)) mask |= 0x0000ff00;
            if(BitUtils.BitTest32(opcode, 16)) mask |= 0x000000ff;

            uint registerValue;
            
            if(spsr) {
                registerValue = cpu.Spsr;
            } else {
                registerValue = cpu.Cpsr;
            }

            registerValue &= ~mask;
            registerValue |= operand & mask;

            if(spsr) {
                cpu.Spsr = registerValue;
            } else {
                cpu.Cpsr = registerValue;
            }
        }

        private static void OpcodeArmPsrTransfer(CPU cpu, uint opcode) {
            bool msr = BitUtils.BitTest32(opcode, 21);
            bool spsr = BitUtils.BitTest32(opcode, 22);

            if(msr) {
                bool immediate = BitUtils.BitTest32(opcode, 25);

                if(immediate) {
                    uint shift = (opcode & 0x00000f00) >> 8;
                    uint imm = opcode & 0x000000ff;

                    uint operand = BitUtils.RotateRight32(imm, (int)shift << 1);

                    OpcodeArmPsrTransfer_Msr(cpu, opcode, spsr, operand);
                } else {
                    if((opcode & 0x00000ff0) != 0x00000000) {
                        cpu.RaiseUND();
                    }

                    uint rm = opcode & 0x0000000f;

                    OpcodeArmPsrTransfer_Msr(cpu, opcode, spsr, cpu.r[rm]);
                }
            } else {
                if((opcode & 0x000f0fff) != 0x000f0000) {
                    cpu.RaiseUND();
                }

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
                    if(rm == 15) {
                        rm_v += 4;
                    }

                    // Shift the register value with a register value
                    if((opcode & 0x00000080) != 0) {
                        throw new InvalidOpcodeException();
                    }

                    uint rs = (opcode & 0x00000f00) >> 8;
                    uint shift = r[rs] & 0x000000ff;

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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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

        private static bool OpcodeArmSubCarry(uint a, uint b) {
            return a >= b;
        }

        private static bool OpcodeArmSubOverflow(uint a, uint b, uint r) {
            return BitUtils.BitTest32(a ^ b, 31) && BitUtils.BitTest32(a ^ r, 31);
        }

        private static void OpcodeArmSub(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                    cpu.flagC = OpcodeArmSubCarry(rn_v, cpu.shifterResult);
                    cpu.flagV = OpcodeArmSubOverflow(rn_v, cpu.shifterResult, result);
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                    cpu.flagC = OpcodeArmSubCarry(cpu.shifterResult, rn_v);
                    cpu.flagV = OpcodeArmSubOverflow(cpu.shifterResult, rn_v, result);
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, result);
        }

        private static bool OpcodeArmAddOverflow(uint a, uint b, uint r) {
            return !BitUtils.BitTest32(a ^ b, 31) && BitUtils.BitTest32(a ^ r, 31);
        }

        private static void OpcodeArmAdd(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                    cpu.flagV = OpcodeArmAddOverflow(rn_v, cpu.shifterResult, result);
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            ulong result = (ulong)rn_v + (ulong)cpu.shifterResult + (cpu.flagC ? 1UL : 0UL);

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.flagC = result > uint.MaxValue;
                    cpu.flagV = OpcodeArmAddOverflow(rn_v, cpu.shifterResult, (uint)result);
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags((uint)result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, (uint)result);
        }

        private static bool OpcodeArmSbcCarry(uint a, uint b, bool c) {
            return a >= b && ((a - b) >= (c ? 0 : 1));
        }

        private static void OpcodeArmSbc(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = rn_v - cpu.shifterResult - (cpu.flagC ? 0U : 1U);

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.flagV = OpcodeArmSubOverflow(rn_v, cpu.shifterResult, result);
                    cpu.flagC = OpcodeArmSbcCarry(rn_v, cpu.shifterResult, cpu.flagC);
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags((uint)result);
                }
            }

            cpu.OpcodeArmDataProcessingWriteRegister(rd, (uint)result);
        }

        private static void OpcodeArmRsc(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;
            bool s = BitUtils.BitTest32(opcode, 20);

            uint result = cpu.shifterResult - rn_v - (cpu.flagC ? 0U : 1U);

            if(s) {
                if(rd == 15) {
                    cpu.Cpsr = cpu.Spsr;
                } else {
                    cpu.flagV = OpcodeArmSubOverflow(cpu.shifterResult, rn_v, result);
                    cpu.flagC = OpcodeArmSbcCarry(cpu.shifterResult, rn_v, cpu.flagC);
                    cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
                }
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;

            uint result = rn_v - cpu.shifterResult;

            if(rd == 15) {
                cpu.Cpsr = cpu.Spsr;
            } else {
                cpu.flagC = OpcodeArmSubCarry(rn_v, cpu.shifterResult);
                cpu.flagV = OpcodeArmSubOverflow(rn_v, cpu.shifterResult, result);
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
                    rn_v += 4;
                }
            }

            uint rd = (opcode & 0x0000f000) >> 12;

            uint result = rn_v + cpu.shifterResult;

            if(rd == 15) {
                cpu.Cpsr = cpu.Spsr;
            } else {
                cpu.flagC = result < rn_v;
                cpu.flagV = OpcodeArmAddOverflow(rn_v, cpu.shifterResult, result);
                cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
            }
        }

        private static void OpcodeArmOrr(CPU cpu, uint opcode) {
            cpu.OpcodeArmDataProcessingShifter(opcode);

            uint rn = (opcode & 0x000f0000) >> 16;
            uint rn_v = cpu.r[rn];

            if(rn == 15) {
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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
                if(!BitUtils.BitTest32(opcode, 25) && BitUtils.BitTest32(opcode, 4)) {
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

            uint offset = (uint)(opcode & 0x00ffffff) << 2;

            if(BitUtils.BitTest32(offset, 25)) {
                offset |= 0xfc000000;
            }
            
            cpu.PerformJump(cpu.r[15] + offset);
        }
    }
}
