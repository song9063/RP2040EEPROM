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
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "bs_pico_at24c.h"

#define AT24C256_ADDR 80
#define TEST_BUFFER_SIZE 100

void init_i2c(i2c_inst_t *i2c){
  i2c_init(i2c, 100 * 1000);
  gpio_set_function(14, GPIO_FUNC_I2C);
  gpio_set_function(15, GPIO_FUNC_I2C);
  gpio_pull_up(14);
  gpio_pull_up(15);
}

int main(){
  stdio_init_all();
  
  sleep_ms(5000);
  i2c_inst_t *i2c = i2c1;
  init_i2c(i2c);

  BS_AT24CDev at24c = {
    .i2c=i2c,
    .i2c_addr=AT24C256_ADDR,
    .page_size=64,
    .pages=512,
    .addr_size=2
  };

  uint8_t buff[TEST_BUFFER_SIZE];
  memset(buff, 0x00, sizeof(buff));

  /* Write/Read a data at index 0 of page 0 */
  bs_pico_at24c_clear_page(&at24c, 0); /* Clear all of the bytes to 0x00 on the page */
  int nSent = bs_pico_at24c_write_at(&at24c, 0, "Hello", 5);
  int nRead = bs_pico_at24c_read_at(&at24c, 0, buff, 5);
  printf("%s (Sent %d bytes, Read %d bytes)\n", buff, nSent, nRead);

  /* Write/Read a data by page */
  memset(buff, 0x00, sizeof(buff));
  bs_pico_at24c_write_page(&at24c, 1, "Page 1");
  bs_pico_at24c_read_page(&at24c, 1, buff);
  printf("Text of page 1: %s\n", buff);

  memset(buff, 0x00, sizeof(buff));
  bs_pico_at24c_write_page(&at24c, 2, "Page 2");
  bs_pico_at24c_read_page(&at24c, 2, buff);
  printf("Text of page 2: %s\n", buff);

  /* Write/Read a long text over the page size, It will split the text by page size. */
  memset(buff, 0x00, sizeof(buff));
  bs_pico_at24c_clear_page(&at24c, 3); /* Clear all of the bytes to 0x00 on the page */
  bs_pico_at24c_clear_page(&at24c, 4); /* Clear all of the bytes to 0x00 on the page */
  snprintf(buff, TEST_BUFFER_SIZE, "Write a long text over the page size, It will split the text by page size.");
  size_t len = strlen(buff);
  nSent = bs_pico_at24c_write_long_to_page(&at24c, 3, buff, len);
  printf("%s(%d bytes sent)\n", buff, nSent);

  memset(buff, 0x00, sizeof(buff));
  bs_pico_at24c_read_page(&at24c, 3, buff);
  printf(buff);

  memset(buff, 0x00, sizeof(buff));
  bs_pico_at24c_read_page(&at24c, 4, buff);
  printf("%s\n", buff);
  printf("Done.\n");
  
  return 0;
}
