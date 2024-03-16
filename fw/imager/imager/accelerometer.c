/*
 * accelerometer.c
 *
 * Created: 21.02.2024 22:10:34
 *  Author: Minty
 *
 * code partially adapted from https://github.com/boschsensortec/BMI160_driver
 */

#include <avr/io.h>
#include <util/delay.h>
#include "BMI160_driver/bmi160_defs.h"
#include "hw.h"


static struct bmi160_dev dev;

uint8_t communicate_spi(uint8_t output) {
	USIDR = output;
	// clear USI CounterOverflow Flag and counter value
	USISR |= (1 << USIOIF);
	while ( !(USISR & _BV(USIOIF)) ) USICR |= _BV(USITC);
	/*const uint8_t clock = USICR | (1 << USITC);
	do {
		USICR = clock;
	} while ((USISR & (1 << USIOIF)) == 0);*/
	return USIDR;
}

int8_t read(uint8_t reg_addr, uint8_t *data, uint8_t len) {
	// set CS active (low)
	ACC_PORT &= ~(1 << ACC_CS);
	// send read bit and starting address
	reg_addr |= BMI160_SPI_RD_MASK;
	communicate_spi(reg_addr);
	for (int i = 0; i < len; ++i) {
		*data = communicate_spi(0x00);
		++data;
	}
	// set CS inactive (high)
	ACC_PORT |= (1 << ACC_CS);
	return BMI160_OK;
}

int8_t write(uint8_t reg_addr, uint8_t *data, uint8_t len) {
	// set CS active (low)
	ACC_PORT &= ~(1 << ACC_CS);
	// send read bit and starting address
	reg_addr &= BMI160_SPI_WR_MASK;
	communicate_spi(reg_addr);
	for (int i = 0; i < len; ++i) {
		communicate_spi(*data);
		++data;
	}
	// set CS inactive (high)
	ACC_PORT |= (1 << ACC_CS);
	return BMI160_OK;
}

/*!
 * @brief This API reads the data from the given register address
 * of sensor.
 */
int8_t bmi160_get_regs(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    int8_t rslt = BMI160_OK;
	if (len == 0) {
        rslt = BMI160_E_READ_WRITE_LENGTH_INVALID;
    } else {
        rslt = read(reg_addr, data, len);
    }
    return rslt;
}

/*!
 * @brief This API writes the given data to the register address
 * of sensor.
 */
int8_t bmi160_set_regs(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    int8_t rslt = BMI160_OK;
    uint8_t count = 0;
    /* Null-pointer check */
    if (len == 0) {
        rslt = BMI160_E_READ_WRITE_LENGTH_INVALID;
    } else {
        if ((dev.prev_accel_cfg.power == BMI160_ACCEL_NORMAL_MODE) || (dev.prev_gyro_cfg.power == BMI160_GYRO_NORMAL_MODE)) {
            rslt = write(reg_addr, data, len);
            /* Kindly refer bmi160 data sheet section 3.2.4 */
            _delay_ms(1);
        } else {
            /*Burst write is not allowed in
             * suspend & low power mode */
            for (; count < len; count++) {
                rslt = write(reg_addr, &data[count], 1);
                reg_addr++;
                /* Kindly refer bmi160 data sheet section 3.2.4 */
                _delay_ms(1);
            }
        }
        if (rslt != BMI160_OK) {
            rslt = BMI160_E_COM_FAIL;
        }
    }
    return rslt;
}

/*!
 * @brief This API sets the default configuration parameters of accel & gyro.
 * Also maintain the previous state of configurations.
 */
static void default_param_settg(struct bmi160_dev *dev)
{
    /* Initializing accel and gyro params with default values */
    dev->accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;
    dev->accel_cfg.odr = BMI160_ACCEL_ODR_100HZ;
    dev->accel_cfg.power = BMI160_ACCEL_SUSPEND_MODE;
    dev->accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    dev->gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;
    dev->gyro_cfg.odr = BMI160_GYRO_ODR_100HZ;
    dev->gyro_cfg.power = BMI160_GYRO_SUSPEND_MODE;
    dev->gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    /* To maintain the previous state of accel configuration */
    dev->prev_accel_cfg = dev->accel_cfg;
    /* To maintain the previous state of gyro configuration */
    dev->prev_gyro_cfg = dev->gyro_cfg;
}


/*!
 * @brief This API resets and restarts the device.
 * All register values are overwritten with default parameters.
 */
int8_t bmi160_soft_reset(struct bmi160_dev *dev)
{
    int8_t rslt;
    uint8_t data = BMI160_SOFT_RESET_CMD;
    /* Null-pointer check */
    if (dev == NULL) {
        rslt = BMI160_E_NULL_PTR;
    } else {
        /* Reset the device */
        rslt = bmi160_set_regs(BMI160_COMMAND_REG_ADDR, &data, 1);
        _delay_ms(BMI160_SOFT_RESET_DELAY_MS);
        if ((rslt == BMI160_OK) && (dev->intf == BMI160_SPI_INTF)) {
            /* Dummy read of 0x7F register to enable SPI Interface if SPI is used */
            rslt = bmi160_get_regs(BMI160_SPI_COMM_TEST_ADDR, &data, 1);
        }

        if (rslt == BMI160_OK)
        {
            /* Update the default parameters */
            default_param_settg(dev);
        }
    }

    return rslt;
}


/*!
 *  @brief This API is the entry point for sensor.It performs
 *  the selection of I2C/SPI read mechanism according to the
 *  selected interface and reads the chip-id of bmi160 sensor.
 */
inline int8_t bmi160_init(struct bmi160_dev *dev) {
    int8_t rslt;
    uint8_t data;
    uint8_t try = 3;
	dev->intf = BMI160_SPI_INTF;
    /* Dummy read of 0x7F register to enable SPI Interface */
    rslt = bmi160_get_regs(BMI160_SPI_COMM_TEST_ADDR, &data, 1);
    /* Make sure chip is responding */
    if (rslt == BMI160_OK) {
        dev->chip_id = 0;
        while ((try--) && (dev->chip_id != BMI160_CHIP_ID)) {
            /* Read chip_id */
			// register 0x00
            rslt = bmi160_get_regs(BMI160_CHIP_ID_ADDR, &dev->chip_id, 1);
        }
        if ((rslt == BMI160_OK) && (dev->chip_id == BMI160_CHIP_ID)) {
            dev->any_sig_sel = BMI160_BOTH_ANY_SIG_MOTION_DISABLED;
            /* Soft reset */
            rslt = bmi160_soft_reset(dev);
        } else {
            rslt = BMI160_E_DEV_NOT_FOUND;
        }
    }
    return rslt;
}

void init_accelerometer() {
	// input: int, miso
	ACC_DDR &= ~((1 << ACC_INT1) | (1 << DDA6));
	// output: cs, sck, mosi
	ACC_DDR |= (1 << ACC_CS) | (1 << DDA4) | (1 << DDA5);
	// init SPI
	// three-wire mode, clock source is external, positive edge, software clock strobe
	USICR = (0 << USIWM1) | (1 << USIWM0) | (1 << USICS1) | (0 << USICS0) | (1 << USICLK);
	/* CSB pin is made high for selecting SPI protocol
     * Note: CSB has to see rising after power up, to switch to SPI protocol */
	ACC_PORT &= ~(1 << ACC_CS);
	_delay_ms(10);
	// set CS inactive
	ACC_PORT |= (1 << ACC_CS);
	// empirical: there should be a delay after CS inactive. It won't communicate anything but 0xFF until then
	_delay_us(150);
	bmi160_init(&dev);
}
