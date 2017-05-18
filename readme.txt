

canRxMessage.id==0X001     开始基频
canRxMessage.id==0x012     开始发送数据
canTxMessage.id=0x101;     自己对应的版号
canTxMessage.id=0x102;     自己对应的版号



                       id                  000 0000 0001
C1RXM0SID=CAN_FILTERMASK2REG_SID(0x7bc);   000 0001 0010    111  1110 1100
1 = 过滤器比较操作包含SIDx 位
C1RXF0SID=CAN_FILTERMASK2REG_SID(0x043);   000 0001 0011    111  1011 1100

canRxMessage.id==0X001     开始基频
canRxMessage.id==0x042     开始发送数据
canTxMessage.id=0x401;     自己对应的版号
canTxMessage.id=0x402;     自己对应的版号


                       id                  000 0000 0001
C1RXM0SID=CAN_FILTERMASK2REG_SID(0x7bc);   000 000 0010    111  1011 1100
1 = 过滤器比较操作包含SIDx 位
C1RXF0SID=CAN_FILTERMASK2REG_SID(0x043);   000 0100 0011    111  1011 1100