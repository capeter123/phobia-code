/*
   Phobia DC Motor Controller for RC and robotics.
   Copyright (C) 2014 Roman Belov <romblv@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_UART_
#define _H_UART_

#define UART_RXBUF_SZ		40
#define UART_TXBUF_SZ		40

typedef struct {

	char		rBuf[UART_RXBUF_SZ];
	int		rN;

	char		tBuf[UART_TXBUF_SZ];

}
halUART_TypeDef;

extern halUART_TypeDef		halUART;

void uartEnable(unsigned long int bR);
void uartDisable();

int uartRX();
char *uartTryTX();
void uartTX(int N);

#endif /* _H_UART_ */

