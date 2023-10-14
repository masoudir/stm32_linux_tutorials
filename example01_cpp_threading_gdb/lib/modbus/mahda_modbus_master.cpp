#include "mahda_modbus_master.h"

/**
 * @brief The constructor function of this class. This will create a modbus master object using the input addresses.
 * @param addr : The input address wh
 */
MahdaModbusMaster::MahdaModbusMaster(ModbusAddresses *addr, modbus_connection_details *connection_details_config) :
        addresses(*addr), connection_details(*connection_details_config), old_response_to_sec(0),
        old_response_to_usec(0), is_ready_to_connect(false), is_connected(
        false) {
    if (addr != nullptr && connection_details_config != nullptr) {
        is_ready_to_connect = true;
    }
}

/**
 * @brief The destructor function of this class.
 */
MahdaModbusMaster::~MahdaModbusMaster() {
    disconnect();
}

/**
 * @brief Connects the Modbus node to the network which must be initialized formerly.
 * @return bool : If the act of connecting to the network was successful it will return true, other this will return
 *                false.
 */
bool MahdaModbusMaster::connect() {
    if (!is_ready_to_connect) {
        return false;
    }
    mtx.lock();
    if (connection_details.type == TCP) {
        ctx = modbus_new_tcp(connection_details.tcp.ip.c_str(), int(connection_details.tcp.port));
    } else if (connection_details.type == TCP_PI) {
        ctx = modbus_new_tcp_pi(connection_details.tcp_pi.node, connection_details.tcp_pi.service);
    } else {
        ctx = modbus_new_rtu(connection_details.rtu.device,
                             int(connection_details.rtu.baudrate),
                             connection_details.rtu.parity,
                             connection_details.rtu.data_bit,
                             connection_details.rtu.stop_bit);

//        ctx = modbus_new_rtu("/dev/pts/1", 115200, 'N', 8, 1);

        modbus_set_slave(ctx, addresses.server_id);
    }

    if (ctx == nullptr) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return false;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_LINK |
                                                                      MODBUS_ERROR_RECOVERY_PROTOCOL));

    modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return false;
    }
    mtx.unlock();
    is_connected = true;
    return true;
}

/**
 * @brief Disconnects the modbus from network.
 */
void MahdaModbusMaster::disconnect() {
    mtx.lock();


    /* Close the connection */
    usleep(400000);
    modbus_flush(ctx);


    modbus_close(ctx);
    modbus_free(ctx);
    mtx.unlock();
}

/**
 * @brief Reads an array of bits from bits buffer
 * @param addr : start address for reading
 * @param nb : the number of bits needed for reading
 * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
 * @return bool : If the act of reading was successful it will return true, otherwise this will return
     *                false.
 */
bool MahdaModbusMaster::modbus_read_bits_by_address(int addr, int nb, uint8_t *dest) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_read_bits(ctx, addr, nb, dest) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

/**
 * @brief Reads input bits using the start address
 * @param addr : start address
 * @param nb : the number of bits needed for reading
 * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
 * @return bool : bool : If the act of reading was successful it will return true, otherwise this will return
     *                false.
 */
bool MahdaModbusMaster::modbus_read_input_bits_by_address(int addr, int nb, uint8_t *dest) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_read_input_bits(ctx, addr, nb, dest) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

/**
 * @brief Reads registers using the start address (each register contains 2 bytes)
 * @param addr : start address
 * @param nb : the number of bits needed for reading
 * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
 * @return bool : If the act of reading was successful it will return true, otherwise this will return
 *                false.
 */
bool MahdaModbusMaster::modbus_read_registers_by_address(int addr, int nb, uint16_t *dest) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_read_registers(ctx, addr, nb, dest) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

/**
 * @brief Reads input registers using the start address (each register contains 2 bytes)
 * @param addr : start address
 * @param nb : the number of bits needed for reading
 * @param dest : pointer to the address of the destination buffer in order to transfer data_base bits.
 * @return bool : If the act of reading was successful it will return true, otherwise this will return
 *                false.
 */
bool MahdaModbusMaster::modbus_read_input_registers_by_address(int addr, int nb, uint16_t *dest) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_read_input_registers(ctx, addr, nb, dest) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

/**
 * @brief Writes data_base into a bit for the input address
 * @param coil_addr : input coil address
 * @param status : data_base content
 * @return bool : If the act of writing was successful it will return true, otherwise this will return
 *                false.
 */
bool MahdaModbusMaster::modbus_write_bit_by_address(int coil_addr, int status) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_write_bit(ctx, coil_addr, status) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

/**
 * @brief Writes data_base into the register for the input address
 * @param reg_addr : input register address
 * @param value : data_base content
 * @return bool : If the act of writing was successful it will return true, otherwise this will return
 *                false.
 */
bool MahdaModbusMaster::modbus_write_register_by_address(int reg_addr, const uint16_t value) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_write_register(ctx, reg_addr, value) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

/**
 * @brief Writes data_base into an array of bits for the input address
 * @param reg_addr : input register address
 * @param status : data_base content
 * @return bool : If the act of writing was successful it will return true, otherwise this will return
 *                false.
 */
bool MahdaModbusMaster::modbus_write_bits_by_address(int addr, int nb, const uint8_t *data) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_write_bits(ctx, addr, nb, data) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

/**
 * @brief Writes data_base into an array of registers (each register contains 2 bytes)
 * @param addr : input start address
 * @param nb : the number of registers needed to be used for writing
 * @param data : pointer to the data_base content
 * @return bool : If the act of writing was successful it will return true, otherwise this will return
 *                false.
 */
bool MahdaModbusMaster::modbus_write_registers_by_address(int addr, int nb, const uint16_t *data) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_write_registers(ctx, addr, nb, data) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

bool MahdaModbusMaster::modbus_mask_write_register_by_address(int addr, uint16_t and_mask, uint16_t or_mask) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_mask_write_register(ctx, addr, and_mask, or_mask) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

bool MahdaModbusMaster::modbus_write_and_read_registers_by_address(int write_addr, int write_nb,
                                                                   const uint16_t *src, int read_addr, int read_nb,
                                                                   uint16_t *dest) {
    bool out = false;
    if(is_connected) {
        mtx.lock();
        if (modbus_write_and_read_registers(ctx, write_addr, write_nb, src, read_addr, read_nb, dest) == 1) {
            out = true;
        }
        mtx.unlock();
    }
    return out;
}

float MahdaModbusMaster::modbus_read_float_registers_by_address(int addr) {
    bool out = false;
    if(is_connected) {
        conv_float_uint16 union_for_conv{};
        mtx.lock();
        if (modbus_read_registers(ctx, addr, 1, &union_for_conv.uint16_arr[0]) == 1) {
            if (modbus_read_registers(ctx, addr + 1, 1, &union_for_conv.uint16_arr[1]) == 1) {
                out = true;
            }
        }
        mtx.unlock();
        if (out) {
            return union_for_conv.f_number;
        }
    }
    return 0.0;
}

float MahdaModbusMaster::modbus_read_float_input_registers_by_address(int addr) {
    bool out = false;
    if(is_connected) {
        conv_float_uint16 union_for_conv{};
        mtx.lock();
        if (modbus_read_input_registers(ctx, addr, 1, &union_for_conv.uint16_arr[0]) == 1) {
            if (modbus_read_input_registers(ctx, addr + 1, 1, &union_for_conv.uint16_arr[1]) == 1) {
                out = true;
            }
        }
        mtx.unlock();
        if (out) {
            return union_for_conv.f_number;
        }
    }
    return 0.0;
}

bool MahdaModbusMaster::modbus_write_float_registers_by_address(int addr, float data) {
    bool out = false;
    if(is_connected) {
        conv_float_uint16 union_for_conv{};
        union_for_conv.f_number = data;
        mtx.lock();
        if (modbus_write_registers(ctx, addr, 1, &union_for_conv.uint16_arr[0]) == 1) {
            if (modbus_write_registers(ctx, addr + 1, 1, &union_for_conv.uint16_arr[1]) == 1) {
                out = true;
            }
        }
        mtx.unlock();
    }
    return out;
}

