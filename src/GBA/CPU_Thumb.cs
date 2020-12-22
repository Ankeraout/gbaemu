namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ThumbOpcodeHandler(CPU cpu, ushort opcode);

        private static ThumbOpcodeHandler[] thumbOpcodeHandlerTable;

        private static void InitThumbDecodeTable() {
            thumbOpcodeHandlerTable = new ThumbOpcodeHandler[1024];

            for(int i = 0; i < 1024; i++) {
                ushort opcode = (ushort)(i << 6);

                switch((opcode & 0xe000) >> 13) {
                    case 0b000:
                    if((opcode & 0x1800) == 0x1800) {
                        if(BitUtils.BitTest16(opcode, 9)) {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbSub;
                        } else {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbAdd;
                        }
                    } else {
                        switch((opcode & 0x1800) >> 11) {
                            case 0b00:
                            thumbOpcodeHandlerTable[i] = OpcodeThumbLsl;
                            break;

                            case 0b01:
                            thumbOpcodeHandlerTable[i] = OpcodeThumbLsr;
                            break;

                            case 0b10:
                            thumbOpcodeHandlerTable[i] = OpcodeThumbAsr;
                            break;
                        }
                    }

                    break;

                    case 0b001:
                    switch((opcode & 0x1800) >> 11) {
                        case 0b00:
                        thumbOpcodeHandlerTable[i] = OpcodeThumbMov;
                        break;

                        case 0b01:
                        thumbOpcodeHandlerTable[i] = OpcodeThumbCmp;
                        break;

                        case 0b10:
                        thumbOpcodeHandlerTable[i] = OpcodeThumbAdd2;
                        break;

                        case 0b11:
                        thumbOpcodeHandlerTable[i] = OpcodeThumbSub2;
                        break;
                    }
                    
                    break;

                    case 0b010:
                    if(BitUtils.BitTest16(opcode, 12)) {
                        if(BitUtils.BitTest16(opcode, 9)) {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbLdrStrh;
                        } else {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbLdrStr;
                        }
                    } else {
                        if(BitUtils.BitTest16(opcode, 11)) {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbLdr;
                        } else {
                            if(BitUtils.BitTest16(opcode, 10)) {
                                if((opcode & 0x0380) == 0x0300) {
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbBx;
                                } else if((opcode & 0x0300) != 0x0300) {
                                    if((opcode & 0x00c0) != 0x0000) {
                                        switch((opcode & 0x0300) >> 8) {
                                            case 0b00:
                                            thumbOpcodeHandlerTable[i] = OpcodeThumbAdd3;
                                            break;

                                            case 0b01:
                                            thumbOpcodeHandlerTable[i] = OpcodeThumbCmp3;
                                            break;
                                            
                                            case 0b10:
                                            thumbOpcodeHandlerTable[i] = OpcodeThumbMov2;
                                            break;
                                        }
                                    }
                                }
                            } else {
                                switch((opcode & 0x03c0) >> 6) {
                                    case 0b0000:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbAnd;
                                    break;
                                    
                                    case 0b0001:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbEor;
                                    break;
                                    
                                    case 0b0010:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbLsl2;
                                    break;
                                    
                                    case 0b0011:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbLsr2;
                                    break;
                                    
                                    case 0b0100:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbAsr2;
                                    break;
                                    
                                    case 0b0101:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbAdc;
                                    break;
                                    
                                    case 0b0110:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbSbc;
                                    break;
                                    
                                    case 0b0111:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbRor;
                                    break;
                                    
                                    case 0b1000:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbTst;
                                    break;
                                    
                                    case 0b1001:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbNeg;
                                    break;
                                    
                                    case 0b1010:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbCmp2;
                                    break;
                                    
                                    case 0b1011:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbCmn;
                                    break;
                                    
                                    case 0b1100:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbOrr;
                                    break;
                                    
                                    case 0b1101:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbMul;
                                    break;
                                    
                                    case 0b1110:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbBic;
                                    break;

                                    case 0b1111:
                                    thumbOpcodeHandlerTable[i] = OpcodeThumbMvn;
                                    break;
                                }
                            }
                        }
                    }

                    break;

                    case 0b011:
                    thumbOpcodeHandlerTable[i] = OpcodeThumbLdrStr2;
                    break;

                    case 0b100:
                    if(BitUtils.BitTest16(opcode, 12)) {
                        thumbOpcodeHandlerTable[i] = OpcodeThumbLdrStr3;
                    } else {
                        thumbOpcodeHandlerTable[i] = OpcodeThumbLdrStrh2;
                    }

                    break;

                    case 0b101:
                    if(BitUtils.BitTest16(opcode, 12)) {
                        if((opcode & 0x0f00) == 0x0000) {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbAdd5;
                        } else if((opcode & 0x0600) == 0x0400) {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbPushPop;
                        }
                    } else {
                        thumbOpcodeHandlerTable[i] = OpcodeThumbAdd4;
                    }

                    break;

                    case 0b110:
                    if(BitUtils.BitTest16(opcode, 12)) {
                        if((opcode & 0x0f00) == 0x0f00) {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbSwi;
                        } else {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbB;
                        }
                    } else {
                        thumbOpcodeHandlerTable[i] = OpcodeThumbLdmStm;
                    }

                    break;

                    case 0b111:
                    if(BitUtils.BitTest16(opcode, 12)) {
                        thumbOpcodeHandlerTable[i] = OpcodeThumbBl;
                    } else {
                        if(!BitUtils.BitTest16(opcode, 11)) {
                            thumbOpcodeHandlerTable[i] = OpcodeThumbB2;
                        }
                    }

                    break;
                }

                if((opcode & 0xf800) == 0x1800) {
                    if(BitUtils.BitTest16(opcode, 9)) {
                        thumbOpcodeHandlerTable[i] = OpcodeThumbSub;
                    } else {
                        thumbOpcodeHandlerTable[i] = OpcodeThumbAdd;
                    }
                } else if((opcode & 0xff00) == 0xbf00) {
                    thumbOpcodeHandlerTable[i] = OpcodeThumbSwi;
                }
            }
        }

        private ThumbOpcodeHandler DecodeThumbOpcode(ushort opcode) {
            return thumbOpcodeHandlerTable[opcode >> 6];
        }

        private static void OpcodeThumbSwi(CPU cpu, ushort opcode) {
            cpu.RaiseSWI();
        }

        private static void OpcodeThumbAdd(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);
            bool i = BitUtils.BitTest16(opcode, 10);
            uint rs_v = cpu.r[rs];

            uint op2;

            if(i) {
                op2 = (uint)(opcode & 0x01c0) >> 6;
            } else {
                ushort rn = (ushort)((opcode & 0x01c0) >> 6);
                op2 = cpu.r[rn];
            }

            uint result = rs_v + op2;
            cpu.flagC = result < op2;
            cpu.flagV = OpcodeArmAddOverflow(rs_v, op2, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbSub(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);
            bool i = BitUtils.BitTest16(opcode, 10);
            uint rs_v = cpu.r[rs];

            uint op2;

            if(i) {
                op2 = (uint)(opcode & 0x01c0) >> 6;
            } else {
                ushort rn = (ushort)((opcode & 0x01c0) >> 6);
                op2 = cpu.r[rn];
            }

            uint result = rs_v - op2;
            cpu.flagC = OpcodeArmSubCarry(rs_v, op2);
            cpu.flagV = OpcodeArmSubOverflow(rs_v, op2, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbLsl(CPU cpu, ushort opcode) {
            ushort offset = (ushort)((opcode & 0x07c0) >> 6);
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];

            if(offset == 0) {
                cpu.r[rd] = rs_v;
            } else {
                cpu.r[rd] = rs_v << offset;
                cpu.flagC = BitUtils.BitTest32(rs_v >> (32 - offset), 0);
            }

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(cpu.r[rd]);
        }

        private static void OpcodeThumbLsr(CPU cpu, ushort opcode) {
            ushort offset = (ushort)((opcode & 0x07c0) >> 6);
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];

            if(offset == 0) {
                cpu.r[rd] = 0;
                cpu.flagC = BitUtils.BitTest32(rs_v, 31);
            } else {
                cpu.r[rd] = rs_v >> offset;
                cpu.flagC = BitUtils.BitTest32(rs_v >> (offset - 1), 0);
            }

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(cpu.r[rd]);
        }

        private static void OpcodeThumbAsr(CPU cpu, ushort opcode) {
            ushort offset = (ushort)((opcode & 0x07c0) >> 6);
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];

            if(offset == 0) {
                cpu.r[rd] = (uint)((int)rs_v >> 31);
                cpu.flagC = BitUtils.BitTest32(rs_v, 31);
            } else {
                cpu.r[rd] = (uint)((int)rs_v >> offset);
                cpu.flagC = BitUtils.BitTest32(rs_v >> (offset - 1), 0);
            }

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(cpu.r[rd]);
        }

        private static void OpcodeThumbMov(CPU cpu, ushort opcode) {
            ushort rd = (ushort)((opcode & 0x0700) >> 8);
            ushort offset = (ushort)(opcode & 0x00ff);

            cpu.r[rd] = offset;

            cpu.flagZ = offset == 0;
            cpu.flagN = false;
        }

        private static void OpcodeThumbCmp(CPU cpu, ushort opcode) {
            ushort rd = (ushort)((opcode & 0x0700) >> 8);
            ushort offset = (ushort)(opcode & 0x00ff);

            uint rd_v = cpu.r[rd];

            uint result = rd_v - offset;
            cpu.flagC = OpcodeArmSubCarry(rd_v, offset);
            cpu.flagV = OpcodeArmSubOverflow(rd_v, offset, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
        }

        private static void OpcodeThumbAdd2(CPU cpu, ushort opcode) {
            ushort rd = (ushort)((opcode & 0x0700) >> 8);
            ushort offset = (ushort)(opcode & 0x00ff);

            uint rd_v = cpu.r[rd];

            uint result = rd_v + offset;
            cpu.flagC = result < offset;
            cpu.flagV = OpcodeArmAddOverflow(rd_v, offset, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbSub2(CPU cpu, ushort opcode) {
            ushort rd = (ushort)((opcode & 0x0700) >> 8);
            ushort offset = (ushort)(opcode & 0x00ff);

            uint rd_v = cpu.r[rd];

            uint result = rd_v - offset;
            cpu.flagC = OpcodeArmSubCarry(rd_v, offset);
            cpu.flagV = OpcodeArmSubOverflow(rd_v, offset, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbAnd(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rs_v & rd_v;

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbEor(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rs_v ^ rd_v;

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbLsl2(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result;

            if(rs_v == 0) {
                result = rd_v;
            } else if(rs_v < 32) {
                result = rd_v << (int)rs_v;
                cpu.flagC = BitUtils.BitTest32(rd_v >> (int)(rs_v - 1), 1);
            } else if(rs_v == 32) {
                result = 0;
                cpu.flagC = BitUtils.BitTest32(rd_v, 0);
            } else {
                result = 0;
                cpu.flagC = false;
            }

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbLsr2(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result;

            if(rs_v == 0) {
                result = rd_v;
            } else if(rs_v < 32) {
                result = rd_v >> (int)rs_v;
                cpu.flagC = BitUtils.BitTest32(rd_v >> (int)(rs_v - 1), 0);
            } else if(rs_v == 32) {
                result = 0;
                cpu.flagC = BitUtils.BitTest32(rd_v, 31);
            } else {
                result = 0;
                cpu.flagC = false;
            }

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbAsr2(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result;

            if(rs_v == 0) {
                result = rd_v;
            } else if(rs_v < 32) {
                result = (uint)((int)rd_v >> (int)rs_v);
                cpu.flagC = BitUtils.BitTest32(rd_v >> (int)(rs_v - 1), 0);
            } else {
                result = (uint)((int)rd_v >> 31);
                cpu.flagC = BitUtils.BitTest32(rd_v, 31);
            }

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbAdc(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            long result = rd_v + rs_v + (cpu.flagC ? 1 : 0);
            cpu.flagC = result > uint.MaxValue;
            cpu.flagV = OpcodeArmAddOverflow(rd_v, rs_v, (uint)result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags((uint)result);

            cpu.r[rd] = (uint)result;
        }

        private static void OpcodeThumbSbc(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rd_v - rs_v - (uint)(cpu.flagC ? 0 : 1);

            cpu.flagV = OpcodeArmSubOverflow(rd_v, rs_v, result);
            cpu.flagC = OpcodeArmSbcCarry(rd_v, rs_v, cpu.flagC);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags((uint)result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbRor(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result;
            int rotation = (int)rs_v & 0x1f;

            if(rs_v == 0) {
                result = rd_v;
            } else if(rotation == 0) {
                result = rd_v;
                cpu.flagC = BitUtils.BitTest32(rd_v, 31);
            } else {
                result = BitUtils.RotateRight32(rd_v, rotation);
                cpu.flagC = BitUtils.BitTest32(rd_v >> (rotation - 1), 0);
            }

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbTst(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rs_v & rd_v;

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
        }

        private static void OpcodeThumbNeg(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];

            uint result = (uint)-rs_v;
            cpu.flagC = OpcodeArmSubCarry(0, rs_v);
            cpu.flagV = OpcodeArmSubOverflow(0, rs_v, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbCmp2(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rd_v - rs_v;
            cpu.flagC = OpcodeArmSubCarry(rd_v, rs_v);
            cpu.flagV = OpcodeArmSubOverflow(rd_v, rs_v, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
        }

        private static void OpcodeThumbCmn(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rd_v + rs_v;
            cpu.flagC = result < rd_v;
            cpu.flagV = OpcodeArmAddOverflow(rd_v, rs_v, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
        }

        private static void OpcodeThumbOrr(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rs_v | rd_v;

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbMul(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rs_v * rd_v;

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbBic(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rd_v & ~rs_v;

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbMvn(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rs_v = cpu.r[rs];

            uint result = ~rs_v;

            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);

            cpu.r[rd] = result;
        }

        private static void OpcodeThumbAdd3(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            if(BitUtils.BitTest16(opcode, 7)) {
                rd += 8;
            }

            if(BitUtils.BitTest16(opcode, 6)) {
                rs += 8;
            }

            cpu.r[rd] += cpu.r[rs];
        }

        private static void OpcodeThumbCmp3(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            if(BitUtils.BitTest16(opcode, 7)) {
                rd += 8;
            }

            if(BitUtils.BitTest16(opcode, 6)) {
                rs += 8;
            }

            uint rs_v = cpu.r[rs];
            uint rd_v = cpu.r[rd];

            uint result = rd_v - rs_v;
            cpu.flagC = OpcodeArmSubCarry(rd_v, rs_v);
            cpu.flagV = OpcodeArmSubOverflow(rd_v, rs_v, result);
            cpu.OpcodeArmDataProcessingArithmeticalSetFlags(result);
        }

        private static void OpcodeThumbMov2(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            if(BitUtils.BitTest16(opcode, 7)) {
                rd += 8;
            }

            if(BitUtils.BitTest16(opcode, 6)) {
                rs += 8;
            }

            cpu.r[rd] = cpu.r[rs];
        }

        private static void OpcodeThumbBx(CPU cpu, ushort opcode) {
            ushort rs = (ushort)((opcode & 0x0038) >> 3);

            if(BitUtils.BitTest16(opcode, 6)) {
                rs += 8;
            }

            uint dest = cpu.r[rs];

            cpu.flagT = BitUtils.BitTest32(dest, 0);
            cpu.PerformJump(dest);
        }

        private static void OpcodeThumbLdr(CPU cpu, ushort opcode) {
            ushort rd = (ushort)((opcode & 0x0700) >> 8);
            ushort immediate = (ushort)(opcode & 0x00ff);

            uint result = (uint)((cpu.r[15] & 0xfffffffc) + (immediate << 2));

            cpu.r[rd] = cpu.gba.Bus.Read32(result);
        }

        private static void OpcodeThumbLdrStr(CPU cpu, ushort opcode) {
            bool l = BitUtils.BitTest16(opcode, 11);
            bool b = BitUtils.BitTest16(opcode, 10);
            ushort ro = (ushort)((opcode & 0x01c0) >> 6);
            ushort rb = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint ro_v = cpu.r[ro];
            uint rb_v = cpu.r[rb];

            uint offset = ro_v + rb_v;

            if(l) {
                if(b) {
                    cpu.r[rd] = cpu.gba.Bus.Read8(offset);
                } else {
                    cpu.r[rd] = BitUtils.RotateRight32(cpu.gba.Bus.Read32(offset), (int)(offset & 0x00000003) << 3);
                }
            } else {
                uint rd_v = cpu.r[rd];

                if(b) {
                    cpu.gba.Bus.Write8(offset, (byte)rd_v);
                } else {
                    cpu.gba.Bus.Write32(offset, rd_v);
                }
            }
        }

        private static void OpcodeThumbLdrStrh(CPU cpu, ushort opcode) {
            bool h = BitUtils.BitTest16(opcode, 11);
            bool s = BitUtils.BitTest16(opcode, 10);
            ushort ro = (ushort)((opcode & 0x01c0) >> 6);
            ushort rb = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint ro_v = cpu.r[ro];
            uint rb_v = cpu.r[rb];

            uint offset = ro_v + rb_v;

            if(s) {
                if(h) {
                    uint result = cpu.gba.Bus.Read16(offset);

                    if(BitUtils.BitTest32(result, 15)) {
                        result |= 0xffff0000;
                    }

                    cpu.r[rd] = result;
                } else {
                    uint result = cpu.gba.Bus.Read8(offset);

                    if(BitUtils.BitTest32(result, 7)) {
                        result |= 0xffffff00;
                    }

                    cpu.r[rd] = result;
                }
            } else {
                if(h) {
                    cpu.r[rd] = cpu.gba.Bus.Read16(offset);
                } else {
                    uint rd_v = cpu.r[rd];
                    cpu.gba.Bus.Write16(offset, (ushort)rd_v);
                }
            }
        }

        private static void OpcodeThumbLdrStr2(CPU cpu, ushort opcode) {
            bool b = BitUtils.BitTest16(opcode, 12);
            bool l = BitUtils.BitTest16(opcode, 11);
            ushort offset = (ushort)((opcode & 0x07c0) >> 6);
            ushort rb = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rb_v = cpu.r[rb];

            if(b) {
                uint addr = (uint)(rb_v + offset);

                if(l) {
                    cpu.r[rd] = cpu.gba.Bus.Read8(addr);
                } else {
                    uint rd_v = cpu.r[rd];
                    cpu.gba.Bus.Write8(addr, (byte)rd_v);
                }
            } else {
                uint addr = (uint)(rb_v + (offset << 2));

                if(l) {
                    cpu.r[rd] = BitUtils.RotateRight32(cpu.gba.Bus.Read32(addr), (int)(addr & 0x00000003) << 3);
                } else {
                    uint rd_v = cpu.r[rd];
                    cpu.gba.Bus.Write32(addr, cpu.r[rd]);
                }
            }
        }

        private static void OpcodeThumbLdrStrh2(CPU cpu, ushort opcode) {
            bool l = BitUtils.BitTest16(opcode, 11);
            ushort offset = (ushort)((opcode & 0x07c0) >> 6);
            ushort rb = (ushort)((opcode & 0x0038) >> 3);
            ushort rd = (ushort)(opcode & 0x0007);

            uint rb_v = cpu.r[rb];

            uint addr = (uint)(rb_v + (offset << 1));

            if(l) {
                cpu.r[rd] = cpu.gba.Bus.Read16(addr);
            } else {
                uint rd_v = cpu.r[rd];
                cpu.gba.Bus.Write16(addr, (ushort)rd_v);
            }
        }

        private static void OpcodeThumbLdrStr3(CPU cpu, ushort opcode) {
            bool l = BitUtils.BitTest16(opcode, 11);
            ushort rd = (ushort)((opcode & 0x0700) >> 8);
            ushort immediate = (ushort)(opcode & 0x00ff);

            uint offset = (uint)(cpu.r[13] + (immediate << 2));

            if(l) {
                cpu.r[rd] = cpu.gba.Bus.Read32(offset);
            } else {
                uint rd_v = cpu.r[rd];
                cpu.gba.Bus.Write32(offset, rd_v);
            }
        }

        private static void OpcodeThumbAdd4(CPU cpu, ushort opcode) {
            bool sp = BitUtils.BitTest16(opcode, 11);
            ushort rd = (ushort)((opcode & 0x0700) >> 8);
            ushort immediate = (ushort)(opcode & 0x00ff);

            if(sp) {
                cpu.r[rd] = (uint)(cpu.r[13] + (immediate << 2));
            } else {
                cpu.r[rd] = (uint)((cpu.r[15] & 0xfffffffc) + (immediate << 2));
            }
        }

        private static void OpcodeThumbAdd5(CPU cpu, ushort opcode) {
            bool s = BitUtils.BitTest16(opcode, 7);
            ushort immediate = (ushort)((opcode & 0x007f) << 2);

            if(s) {
                cpu.r[13] -= immediate;
            } else {
                cpu.r[13] += immediate;
            }
        }

        private static void OpcodeThumbPushPop(CPU cpu, ushort opcode) {
            bool l = BitUtils.BitTest16(opcode, 11);
            bool r = BitUtils.BitTest16(opcode, 8);
            byte rlist = (byte)opcode;
            uint rcount = BitUtils.HammingWeight8(rlist);

            if(r) {
                rcount++;
            }

            uint addr;

            if(l) {
                addr = cpu.r[13];
                cpu.r[13] += rcount * 4;
            } else {
                cpu.r[13] -= rcount * 4;
                addr = cpu.r[13];
            }

            for(int i = 0; i < 8; i++) {
                if(BitUtils.BitTest8(rlist, i)) {
                    if(l) {
                        cpu.r[i] = cpu.gba.Bus.Read32(addr);
                    } else {
                        cpu.gba.Bus.Write32(addr, cpu.r[i]);
                    }

                    addr += 4;
                }
            }

            if(r) {
                if(l) {
                    cpu.PerformJump(cpu.gba.Bus.Read32(addr));
                } else {
                    cpu.gba.Bus.Write32(addr, cpu.r[14]);
                }
            }
        }

        private static void OpcodeThumbLdmStm(CPU cpu, ushort opcode) {
            bool l = BitUtils.BitTest16(opcode, 11);
            ushort rb = (ushort)((opcode & 0x0700) >> 8);
            byte rlist = (byte)opcode;

            for(int i = 0; i < 7; i++) {
                if(BitUtils.BitTest8(rlist, i)) {
                    if(l) {
                        cpu.r[i] = cpu.gba.Bus.Read32(cpu.r[rb]);
                    } else {
                        cpu.gba.Bus.Write32(cpu.r[rb], cpu.r[i]);
                    }
                    
                    cpu.r[rb] += 4;
                }
            }
        }

        private static void OpcodeThumbB(CPU cpu, ushort opcode) {
            Condition condition = (Condition)((opcode & 0x0f00) >> 8);
            sbyte immediate = (sbyte)opcode;
            
            if(cpu.CheckCondition(condition)) {
                cpu.PerformJump(cpu.r[15] + (uint)(immediate << 1));
            }
        }

        private static void OpcodeThumbB2(CPU cpu, ushort opcode) {
            uint immediate = (uint)(opcode & 0x07ff);

            if(BitUtils.BitTest16(opcode, 10)) {
                immediate |= 0xfffff800;
            }

            cpu.PerformJump(cpu.r[15] + (immediate << 1));
        }

        private static void OpcodeThumbBl(CPU cpu, ushort opcode) {
            bool h = BitUtils.BitTest16(opcode, 11);
            uint offset = (ushort)(opcode & 0x07ff);

            if(h) {
                cpu.r[14] += (uint)(offset << 1);

                uint pc_v = cpu.r[15];

                cpu.PerformJump(cpu.r[14]);

                cpu.r[14] = (pc_v - 2) | 0x00000001;
            } else {
                if(BitUtils.BitTest16(opcode, 10)) {
                    offset |= 0xfffff800;
                }

                cpu.r[14] = cpu.r[15] + (uint)(offset << 12);
            }
        }
    }
}
