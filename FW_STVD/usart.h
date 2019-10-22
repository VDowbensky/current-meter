//Status register - read only
hbit TXE = USART1_SR^7;
hbit TC = USART1_SR^6;
hbit RXNE = USART1_SR^5;
hbit IDLE = USART1_SR^4;
hbit OR = USART1_SR^3;
hbit NF = USART1_SR^2;
hbit FE = USART1_SR^1;
hbit PE = USART1_SR^0;

//Control register 1 
hbit R8 = USART1_CR1^7;
hbit T8 = USART1_CR1^6;
hbit USARTD = USART1_CR1^5;
hbit M = USART1_CR1^4;
hbit WAKE = USART1_CR1^3;
hbit PCEN = USART1_CR1^2;
hbit PS = USART1_CR1^1;
hbit PIEN = USART1_CR1^0;

//Control register 2
hbit TIEN = USART1_CR2^7;
hbit TCIEN = USART1_CR2^6;
hbit RIEN = USART1_CR2^5;
hbit ILIEN = USART1_CR2^4;
hbit TEN = USART1_CR2^3;
hbit REN = USART1_CR2^2;
hbit RWU = USART1_CR2^1;
hbit SBK = USART1_CR2^0;

//Control register 3
hbit STOP1 = USART1_CR3^5;
hbit STOP0 = USART1_CR3^4;
hbit CLKEN = USART1_CR3^3;
hbit CPOL = USART1_CR3^2;
hbit CPHA = USART1_CR3^1;
hbit LBCL = USART1_CR3^0;

//Control register 5
hbit DMAT = USART1_CR5^7;
hbit DMAR = USART1_CR5^6;
hbit SCEN = USART1_CR5^5;
hbit NACK = USART1_CR5^4;
hbit HDSEL = USART1_CR5^3;
hbit IRLP = USART1_CR5^2;
hbit IREN = USART1_CR5^1;
hbit EIE = USART1_CR5^0;


