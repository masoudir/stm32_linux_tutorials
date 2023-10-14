//
// Created by mi on 11/1/21.
//

#ifndef MAHDA_MODBUS_MASTER_H
#define MAHDA_MODBUS_MASTER_H


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus.h>
#include <mutex>

#include "mahda_modbus.h"

/**
 * @brief Contains all data_base and functions in order to be used as Modbus Master (Modbus client).
 */
class MahdaModbusMaster {
private:

    ModbusAddresses addresses;      /* The addresses to different sections in the Modbus network. */

    modbus_t *ctx = nullptr;        /* Pointer to the Modbus device object */

    uint32_t old_response_to_sec;   /* The last response time in terms of second */
    uint32_t old_response_to_usec;  /* The last response time in terms of micro-seconds */
    mutable std::mutex mtx;         /* The mutex which has been used to limit access to the instances of this class */


public:
    modbus_connection_details connection_details;   /* Stores all the details of the Modbus connection */
    bool is_ready_to_connect;                       /* It can specifiy if all the details about the connection has been
                                                    * configured or not.
                                                    *   - false : The connection details is not configured yet.
                                                    *   - true  : The connection details is configured and is ready to
                                                    *             call 'connect' function. */
public:
    uint8_t *tab_rp_bits = nullptr;
    uint16_t *tab_rp_registers = nullptr;
    int nb_points;

private:
    bool is_connected;
    /**
     * @brief This function checks the 32-bits value with two 16-bits registers. The first register will be checked with
     *        the most 16-bits valuable part of the input value and the next one will be checked with the least 16-bits
     *        of the input value.
     * @param tab_reg : a pointer to a 16-bits register which must have the size of 2
     * @param value : a 32-bits input value
     * @return int : If the value equals with the registers, then it wll return 1 otherwise 0.
     */
    int equal_dword(uint16_t *tab_reg, uint32_t value);

    /**
     * @brief Allocates memory for the first use.
     */
    void allocate_memory();


public:

    /**
     * @brief The constructor function of this class. This will create a modbus master object using the input addresses.
     * @param addr : The input address wh
     */
    explicit MahdaModbusMaster(ModbusAddresses* addr, modbus_connection_details* connection_details_config);

    /**
     * @brief The destructor function of this class.
     */
    ~MahdaModbusMaster();

    /**
     * @brief Connects the Modbus node to the network which must be initialized formerly.
     * @return bool : If the act of connecting to the network was successful it will return true, other this will return
     *                false.
     */
    bool connect();

    /**
     * @brief Disconnects the modbus from network.
     */
    void disconnect();

    /**
     * @brief Reads an array of bits from bits buffer
     * @param addr : start address for reading
     * @param nb : the number of bits needed for reading
     * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
     * @return bool : If the act of reading was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_read_bits_by_address(int addr, int nb, uint8_t *dest);

    /**
     * @brief Reads input bits using the start address
     * @param addr : start address
     * @param nb : the number of bits needed for reading
     * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
     * @return bool : If the act of reading was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_read_input_bits_by_address(int addr, int nb, uint8_t *dest);

    /**
     * @brief Reads registers using the start address (each register contains 2 bytes)
     * @param addr : start address
     * @param nb : the number of bits needed for reading
     * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
     * @return bool : If the act of reading was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_read_registers_by_address(int addr, int nb, uint16_t *dest);

    /**
     * @brief Reads input registers using the start address (each register contains 2 bytes)
     * @param addr : start address
     * @param nb : the number of bits needed for reading
     * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
     * @return bool : If the act of reading was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_read_input_registers_by_address(int addr, int nb, uint16_t *dest);

    /**
     * @brief Writes data_base into a bit for the input address
     * @param coil_addr : input coil address
     * @param status : data_base content
     * @return bool : If the act of writing was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_write_bit_by_address(int coil_addr, int status);

    /**
     * @brief Writes data_base into the register for the input address
     * @param reg_addr : input register address
     * @param value : data_base content
     * @return bool : If the act of writing was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_write_register_by_address(int reg_addr, const uint16_t value);

    /**
     * @brief Writes data_base into an array of bits for the input address
     * @param reg_addr : input register address
     * @param status : data_base content
     * @return bool : If the act of writing was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_write_bits_by_address(int addr, int nb, const uint8_t *data);

    /**
     * @brief Writes data_base into an array of registers (each register contains 2 bytes)
     * @param addr : input start address
     * @param nb : the number of registers needed to be used for writing
     * @param data : pointer to the data_base content
     * @return bool : If the act of writing was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_write_registers_by_address(int addr, int nb, const uint16_t *data);

    /**
     * @brief Writes data_base into the input register address using mask
     * @param addr : start address
     * @param and_mask : AND mask
     * @param or_mask : OR mask
     * @return bool : If the act of writing was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_mask_write_register_by_address(int addr, uint16_t and_mask, uint16_t or_mask);

    /**
     * @brief Writes and reads an array of registers from the source (pointer) to the destination (pointer)
     * @param write_addr
     * @param write_nb
     * @param src
     * @param read_addr
     * @param read_nb
     * @param dest
     * @return bool : If the act of writing was successful it will return true, otherwise this will return
     *                false.
     */
    bool modbus_write_and_read_registers_by_address(int write_addr, int write_nb,
                                                    const uint16_t *src, int read_addr, int read_nb,
                                                    uint16_t *dest);

    /**
     *
     * @param addr
     * @return
     */
    float modbus_read_float_registers_by_address(int addr);

    /**
     *
     * @param addr
     * @return
     */
    float modbus_read_float_input_registers_by_address(int addr);

    /**
     *
     * @param addr
     * @param data
     * @return
     */
    bool modbus_write_float_registers_by_address(int addr, float data);


/*
private:
    friend class MahdaModbusMasterWithTest;
*/

public:

    void test_timeout();

    void test_read_write_coil_bits();

    void test_read_write_multiple_bits();

    void test_read_write_discrete_inputs();

    void test_read_write_holding_register_single();

    void test_read_write_holding_register_multiple_registers();

    void test_input_registers();

    void test_illegal_address();


    void test_too_many_data();

    void test_slave_reply();

};


#endif //MAHDA_MODBUS_MASTER_H
