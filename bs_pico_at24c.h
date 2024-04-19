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

#ifndef _BS_PICO_AT24C_H_
#define _BS_PICO_AT24C_H_

#include "hardware/i2c.h"


#define BS_PICO_AT24C_I2C_SLEEP_MS 10

typedef struct _BS_AT24C_Dev{
  i2c_inst_t *i2c;
  uint8_t i2c_addr;
  
  uint16_t page_size;
  uint16_t pages;
  uint8_t addr_size;
} BS_AT24CDev;


/* Random Access */
int bs_pico_at24c_write_at(const BS_AT24CDev *dev,
			   const uint16_t mem_start_addr, const uint8_t *data, const size_t len);
int bs_pico_at24c_read_at(const BS_AT24CDev *dev,
			  const uint16_t mem_start_addr, uint8_t *buff, const size_t len);

/* Page Access */
bool bs_pico_at24c_clear_page(const BS_AT24CDev *dev, const uint16_t page);
int bs_pico_at24c_write_page(const BS_AT24CDev *dev,
			     const uint16_t page, const uint8_t *data);
int bs_pico_at24c_write_long_to_page(const BS_AT24CDev *dev,
				     const uint16_t page, const uint8_t *data, const size_t len);
int bs_pico_at24c_read_page(const BS_AT24CDev *dev,
			    const uint16_t page, uint8_t *buff);

#endif
