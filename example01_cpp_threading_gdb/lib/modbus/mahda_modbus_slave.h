//
// Created by mi on 11/1/21.
//

#ifndef MAHDA_MODBUS_SLAVE_H
#define MAHDA_MODBUS_SLAVE_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus.h>

#ifdef _WIN32
# include <winsock2.h>
#else

# include <sys/socket.h>

#endif

/* For MinGW */
#ifndef MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif

#include "mahda_modbus.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class MahdaModbusSlaveData {
public:

    MahdaModbusSlaveData(int input_nb_bits, int input_nb_input_bits, int input_nb_input_registers,
                         int input_nb_registers);

    ~MahdaModbusSlaveData();

    bool is_inited;

    int nb_bits;
    int nb_input_bits;
    int nb_input_registers;
    int nb_registers;
    uint8_t *tab_bits;
    uint8_t *tab_input_bits;
    uint16_t *tab_input_registers;
    uint16_t *tab_registers;
};

/**
 * @brief The modbus slave class.
 */
class MahdaModbusSlave {
public:
    bool update_memory_from_database();
    bool update_database_from_memory();
    bool update_input_data_memory_from_database();
private:
    MahdaModbusSlaveData *data_base;
    modbus_mapping_t *mb_mapping{};
    int s = -1;
    modbus_t *ctx{};
    int rc{};
    uint8_t *query{};

    ModbusAddresses addresses;          /* stores all related modbus addresses */

    /**
     * @brief Initializes the memory for the first use.
     */
    bool initialize_memory();



    /**
     * @brief Allocates the memory for the first use.
     * @return bool : if it fails return false, otherwise it will return true.
     */
    bool allocate_memory();

    /**
     * @brief Processes one incoming request from the received data_base.
     * @return bool : if it fails return false, otherwise it will return true.
     */
    bool process_one_request();

    /**
     * @brief This will create a reply message for the incoming request.
     * @return int
     */
    int reply_to_the_last_request();

public:
    modbus_connection_details connection_details;   /* Contains all the details about connection */
    bool is_connected_to_master;                    /* determines if the slave node is connected to master or not */
    bool is_ready_to_connect;                   /* determines if the node slave is ready to call the connect function */

public:

    /**
     * @brief Constructor function for this class which takes the related modbus addresses and initiates all instances.
     * @param addr : input modbus addresses
     */
    explicit MahdaModbusSlave(ModbusAddresses *addr, modbus_connection_details *connection_details_config,
                              MahdaModbusSlaveData *data_p);

    /**
     * @brief Destructor function
     */
    ~MahdaModbusSlave();

    /**
     * @brief This function will generate a string which is equal to the IP assigned to the input interface.
     *        The generated string will be placed automatically inside the <connection_details.tcp.ip> variable.
     * @param net_interface_name : The name of the network inteface
     * @return If this function found a correct IP address assigned to the network interface, it will return true,
     *         otherwise it will return false.
     */
    bool generate_tcp_ip_string(const char *net_interface_name);

    /**
     * @brief This functions will manually place the input text (as IP address) into <connection_details.tcp.ip>.
     * @param ip : Input IP address
     */
    void set_ip_port_manually(const char *ip);

    /**
     * @brief If <is_ready_to_connect> is true, then you can call this function in order to connect the slave node to
     *        master node.
     * @return bool : If the connection has been established then it will return true, otherwise it will return false.
     */
    bool connect();

    /**
     * @brief Disconnects the slave node from the master node.
     */
    void disconnect();

    /**
     * @brief This will try to process the incoming data_base
     * @return
     */
    bool listen_for_a_new_request();


    uint8_t get_tab_bits_by_index(int index);

    uint8_t get_tab_input_bits_by_index(int index);

    uint16_t get_tab_input_registers_by_index(int index);

    uint16_t get_tab_registers_by_index(int index);

    bool set_tab_bits_by_index(int index, uint8_t data);

    bool set_tab_input_bits_by_index(int index, uint8_t data);

    bool set_tab_input_registers_by_index(int index, uint16_t data);

    bool set_tab_registers_by_index(int index, uint16_t data);

    float get_float_from_input_registers_by_index(int index);

    bool set_float_on_input_registers_by_index(int index, float data);

    float get_float_from_registers_by_index(int index);

    bool set_float_on_registers_by_index(int index, float data);


    uint8_t get_tab_bits_by_address(uint16_t addr);

    uint8_t get_tab_input_bits_by_address(uint16_t addr);

    uint16_t get_tab_input_registers_by_address(uint16_t addr);

    uint16_t get_tab_registers_by_address(uint16_t addr);

    bool set_tab_bits_by_address(uint16_t addr, uint8_t data);

    bool set_tab_input_bits_by_address(uint16_t addr, uint8_t data);

    bool set_tab_input_registers_by_address(uint16_t addr, uint16_t data);

    bool set_tab_registers_by_address(uint16_t addr, uint16_t data);

    float get_float_from_input_registers_by_address(uint16_t start_addr);

    bool set_float_on_input_registers_by_address(uint16_t start_addr, float data);

    float get_float_from_registers_by_address(uint16_t start_addr);

    bool set_float_on_registers_by_address(uint16_t start_addr, float data);


};


#endif //MAHDA_MODBUS_SLAVE_H
