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

#include "bs_pico_at24c.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

bool bs_pico_at24c_write_addr(const BS_AT24CDev *dev, const uint16_t addr);
void bs_pico_at24c_set_addr(const BS_AT24CDev *dev, uint8_t *buff, const uint16_t addr);

bool bs_pico_at24c_write_addr(const BS_AT24CDev *dev, const uint16_t addr){
  uint8_t addrBuff[dev->addr_size];
  bs_pico_at24c_set_addr(dev, addrBuff, addr);
  bool bRet = i2c_write_blocking(dev->i2c, dev->i2c_addr,
				 addrBuff, dev->addr_size, true) == dev->addr_size;
  sleep_ms(BS_PICO_AT24C_I2C_SLEEP_MS);
  return bRet;
}

void bs_pico_at24c_set_addr(const BS_AT24CDev *dev, uint8_t *buff, const uint16_t addr){
  if(dev->addr_size == 2){
    buff[0] = (addr >> 8) & 0xFF;
    buff[1] = addr & 0xFF;
  }else{
    buff[0] = addr & 0xFF;
  }
}

bool bs_pico_at24c_clear_page(const BS_AT24CDev *dev, const uint16_t page){
  if(page < 0 || page >= dev->pages)
    return false;

  uint8_t buff[dev->page_size + dev->addr_size];
  memset(buff, 0x00, sizeof(buff)); // Fill 0x00 to unused bytes;
  bs_pico_at24c_set_addr(dev, buff, page * dev->page_size);

  int nSent = i2c_write_blocking(dev->i2c, dev->i2c_addr, buff, dev->page_size + dev->addr_size, false);
  sleep_ms(BS_PICO_AT24C_I2C_SLEEP_MS);  
  return nSent > 0;
}

int bs_pico_at24c_write_page(const BS_AT24CDev *dev,
			     const uint16_t page, const uint8_t *data){
  if(page < 0 || page >= dev->pages)
    return -1;

  uint16_t addr = page * dev->page_size;
  uint8_t buff[dev->page_size];
  memset(buff, 0x00, sizeof(buff)); // Fill 0x00 to unused bytes;
  snprintf(buff, dev->page_size, "%s", data);
  return bs_pico_at24c_write_at(dev, addr, buff, dev->page_size);
}

int bs_pico_at24c_write_long_to_page(const BS_AT24CDev *dev,
				     const uint16_t page, const uint8_t *data, const size_t len){
  if(page < 0 || page >= dev->pages)
    return -1;
  if((page * dev->page_size + len - 1) >= (dev->page_size * dev->pages))
    return -1;

  uint16_t nAddrToPage = page * dev->page_size;
  uint16_t nPages = (uint16_t)(len / dev->page_size);
  uint16_t nLastPageLen = len % dev->page_size;
  if(nLastPageLen > 0)
    nPages++;

  uint8_t buff[dev->page_size+dev->addr_size]; // Address + Size of page
  int nSent = 0;
  for(uint16_t nPage=0; nPage < nPages; nPage++){
    const bool bLastPage = nPage == (nPages-1);
    const uint16_t nPerPage = bLastPage ? nLastPageLen : dev->page_size;
    const uint16_t data_start_index = nPage * dev->page_size;
    
    memset(buff, 0x00, sizeof(buff));
    bs_pico_at24c_set_addr(dev, buff, data_start_index + nAddrToPage);
    strncpy(buff+dev->addr_size, data+data_start_index, nPerPage);
    
    nSent += i2c_write_blocking(dev->i2c, dev->i2c_addr, buff, nPerPage + dev->addr_size, false);
    sleep_ms(BS_PICO_AT24C_I2C_SLEEP_MS);
  }
  return nSent;
}

int bs_pico_at24c_write_at(const BS_AT24CDev *dev,
			   const uint16_t mem_start_addr, const uint8_t *data, const size_t len){
  if((mem_start_addr + len - 1) >= (dev->page_size * dev->pages)){
    return -1;
  }

  uint8_t buff[dev->page_size+dev->addr_size];
  bs_pico_at24c_set_addr(dev, buff, mem_start_addr);
  snprintf(buff + dev->addr_size, dev->page_size, "%s", data);
  int nSent = i2c_write_blocking(dev->i2c, dev->i2c_addr, buff, len+dev->addr_size, false);
  sleep_ms(BS_PICO_AT24C_I2C_SLEEP_MS);
  return nSent;
}

int bs_pico_at24c_read_at(const BS_AT24CDev *dev,
			  const uint16_t mem_start_addr, uint8_t *buff, const size_t len){
  if((mem_start_addr + len - 1) >= (dev->page_size * dev->pages)){
    return -1;
  }
  if(bs_pico_at24c_write_addr(dev, mem_start_addr)){
    sleep_ms(BS_PICO_AT24C_I2C_SLEEP_MS);
    int nSent = i2c_read_blocking(dev->i2c, dev->i2c_addr, buff, len, false);
    sleep_ms(BS_PICO_AT24C_I2C_SLEEP_MS);
    return nSent;
  }
  return -1;
}

int bs_pico_at24c_read_page(const BS_AT24CDev *dev,
			    const uint16_t page, uint8_t *buff){
  if(page < 0 || page >= dev->pages)
    return -1;

  uint16_t addr = page * dev->page_size;
  return bs_pico_at24c_read_at(dev, addr, buff, dev->page_size);
}
