/*
// https://github.com/song9063/RP2040EEPROM
//
// This program is free software: you can redistribute it 
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3 of 
// the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
// See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License 
// along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>
#include "bs_pico_at24c.h"

void bs_pico_at24c_test_write_at(const BS_AT24CDev *dev){
  uint16_t loop_cnt = dev->pages;
  uint8_t wBuff[16];
  memset(wBuff, 0x00, sizeof(wBuff));  
  for(uint16_t addr=0; addr<loop_cnt; addr++){
    snprintf(wBuff, 16, "Hello EEP %05d", addr);
    uint16_t eepAddr = addr * dev->page_size;
    int nSent = bs_pico_at24c_write_at(dev, eepAddr, wBuff, 15);
    printf("Write %d(%d)\n", eepAddr, nSent);
    printf("%s\n", wBuff);
    sleep_ms(500);
  }
  printf("Write Done.\n");

  int nReadErrCnt = 0;
  for(uint16_t addr=0; addr<loop_cnt; addr++){
    memset(wBuff, 0x00, sizeof(wBuff));
    uint16_t eepAddr = addr * dev->page_size;
    int nRead = bs_pico_at24c_read_at(dev, eepAddr, wBuff, 15);
    if(nRead > -1){
      printf("Read %s(len:%d, at %d)\n", wBuff, nRead, eepAddr);
    }else{
      printf("Read Error at %d\n", eepAddr);
      nReadErrCnt++;
    }

    if(addr % 32 == 0){
      printf("\n");
      sleep_ms(1000);
    }
  }
  printf("Total Error: %d\n", nReadErrCnt);
}
