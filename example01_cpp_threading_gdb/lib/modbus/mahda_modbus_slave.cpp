
#include "mahda_modbus_slave.h"
#include <thread>

MahdaModbusSlaveData::MahdaModbusSlaveData(int input_nb_bits, int input_nb_input_bits, int input_nb_input_registers,
                                           int input_nb_registers) : is_inited(false), nb_bits(input_nb_bits),
                                                                     nb_input_bits(input_nb_input_bits),
                                                                     nb_input_registers(input_nb_input_registers),
                                                                     nb_registers(input_nb_registers) {
    tab_bits = new uint8_t[nb_bits];
    tab_input_bits = new uint8_t[nb_input_bits];
    tab_input_registers = new uint16_t[nb_input_registers];
    tab_registers = new uint16_t[nb_registers];

    memset(tab_bits, 0, nb_bits);
    memset(tab_input_bits, 0, nb_input_bits);
    memset(tab_input_registers, 0, nb_input_registers);
    memset(tab_registers, 0, nb_registers);

    is_inited = true;
}

MahdaModbusSlaveData::~MahdaModbusSlaveData() {
    delete tab_bits;
    delete tab_input_bits;
    delete tab_input_registers;
    delete tab_registers;
}

/**
 * @brief Initializes the memory for the first use.
 */
bool MahdaModbusSlave::initialize_memory() {
    if (data_base->is_inited) {
        mb_mapping->tab_bits = new uint8_t[mb_mapping->nb_bits];
        mb_mapping->tab_input_bits = new uint8_t[mb_mapping->nb_input_bits];
        mb_mapping->tab_input_registers = new uint16_t[mb_mapping->nb_input_registers];
        mb_mapping->tab_registers = new uint16_t[mb_mapping->nb_registers];
        update_memory_from_database();
        return true;
    }
//    memset(mb_mapping->tab_input_registers, 0,
//           addresses.ut_input_registres_nb * sizeof(mb_mapping->tab_input_registers[0]));

//            modbus_set_bits_from_bytes(mb_mapping->tab_input_bits, i, addresses.ut_input_bits_nb,
//                                       &data_base.tab_input_bits[i]);
return false;
}

bool MahdaModbusSlave::update_memory_from_database() {
    unsigned int i;
    if (data_base->is_inited && mb_mapping != nullptr) {
        for (i = 0; i < mb_mapping->nb_bits; ++i) {
            mb_mapping->tab_bits[i] = data_base->tab_bits[i];
        }
        for (i = 0; i < mb_mapping->nb_input_bits; ++i) {
            mb_mapping->tab_input_bits[i] = data_base->tab_input_bits[i];
        }
        for (i = 0; i < mb_mapping->nb_input_registers; ++i) {
            mb_mapping->tab_input_registers[i] = data_base->tab_input_registers[i];
        }
        for (i = 0; i < mb_mapping->nb_registers; ++i) {
            mb_mapping->tab_registers[i] = data_base->tab_registers[i];
        }
    }
    return true;
}

bool MahdaModbusSlave::update_input_data_memory_from_database() {
    unsigned int i;
    if (data_base->is_inited && mb_mapping != nullptr) {
        for (i = 0; i < mb_mapping->nb_input_bits; ++i) {
            mb_mapping->tab_input_bits[i] = data_base->tab_input_bits[i];
        }
        for (i = 0; i < mb_mapping->nb_input_registers; ++i) {
            mb_mapping->tab_input_registers[i] = data_base->tab_input_registers[i];
        }
    }
    return true;
}

bool MahdaModbusSlave::update_database_from_memory() {
    unsigned int i;
    if (data_base != nullptr && data_base->is_inited && is_connected_to_master) {
        for (i = 0; i < mb_mapping->nb_bits; ++i) {
            data_base->tab_bits[i] = mb_mapping->tab_bits[i];
        }
//        for (i = 0; i < mb_mapping->nb_input_bits; ++i) {
//            data_base->tab_input_bits[i] = mb_mapping->tab_input_bits[i];
//        }
//        for (i = 0; i < mb_mapping->nb_input_registers; ++i) {
//            data_base->tab_input_registers[i] = mb_mapping->tab_input_registers[i];
//        }
        for (i = 0; i < mb_mapping->nb_registers; ++i) {
            data_base->tab_registers[i] = mb_mapping->tab_registers[i];
        }
    }
    return true;
}

/**
 * @brief Allocates the memory for the first use.
 * @return bool : if it fails return false, otherwise it will return true.
 */
bool MahdaModbusSlave::allocate_memory() {

    mb_mapping = modbus_mapping_new_start_address(
            addresses.ut_bits_address, addresses.ut_bits_nb,
            addresses.ut_input_bits_address, addresses.ut_input_bits_nb,
            addresses.ut_registers_address, addresses.ut_registers_nb_max,
            addresses.ut_input_registers_address, addresses.ut_input_registres_nb);
    if (mb_mapping == nullptr) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return false;
    }

    return initialize_memory();
}

/**
 * @brief Processes one incoming request from the received data_base.
 * @return bool : if it fails return false, otherwise it will return true.
 */
bool MahdaModbusSlave::process_one_request() {
    do {
        rc = modbus_receive(ctx, query);
        /* Filtered queries return 0 */
    } while (rc == 0);

    /* The connection is not closed on errors which require on reply such as
   bad CRC in RTU. */
    if (rc == -1 && errno != EMBBADCRC) {
        /* Quit */
        printf("process vv \n");
        return false;
    }
    rc = reply_to_the_last_request();
    if (rc == -1) {
        printf("Quit the loop due to error: %s\n", modbus_strerror(errno));
        return false;
    }
    return true;
}

/**
 * @brief This will create a reply message for the incoming request.
 * @return int
 */
int MahdaModbusSlave::reply_to_the_last_request() {
    return modbus_reply(ctx, query, rc, mb_mapping);
}

/**
 * @brief This function will generate a string which is equal to the IP assigned to the input interface.
 *        The generated string will be placed automatically inside the <connection_details.tcp.ip> variable.
 * @param net_interface_name : The name of the network inteface
 * @return If this function found a correct IP address assigned to the network interface, it will return true,
 *         otherwise it will return false.
 */
bool MahdaModbusSlave::generate_tcp_ip_string(const char *net_interface_name) {
    struct ifaddrs *ifaddr, *ifa;
    int family, tcp_s;
    char ip[15] = "";
    connection_details.tcp.ip.clear();

    if (getifaddrs(&ifaddr) == -1) {
//            perror("getifaddrs");
//            exit(EXIT_FAILURE);
        return false;
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            tcp_s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                                ip, 15, nullptr, 0, NI_NUMERICHOST);
            if (tcp_s != 0) {
//                    printf("getnameinfo() failed: %s\n", gai_strerror(tcp_s));
//                    exit(EXIT_FAILURE);
                return false;
            }
            if (strcmp(ifa->ifa_name, net_interface_name) == 0) {
//                printf("JJ <Interface>: %s \t <Address> %s\n", ifa->ifa_name, connection_details.tcp.ip);
                connection_details.tcp.ip.insert(0, ip);
                return true;
            } else {
//                    printf("<Interface>: %s \t <Address> %s\n", ifa->ifa_name, host);
            }
        }
    }
    return false;
}

/**
 * @brief This functions will manually place the input text (as IP address) into <connection_details.tcp.ip>.
 * @param ip : Input IP address
 */
void MahdaModbusSlave::set_ip_port_manually(const char *ip) {
    connection_details.tcp.ip = ip;
}

/**
 * @brief Constructor function for this class which takes the related modbus addresses and initiates all instances.
 * @param addr : input modbus addresses
 */
MahdaModbusSlave::MahdaModbusSlave(ModbusAddresses *addr, modbus_connection_details *connection_details_config,
                                   MahdaModbusSlaveData *data_p) :
        addresses(*addr), connection_details(*connection_details_config), is_ready_to_connect(false),
        is_connected_to_master(false), data_base(data_p) {
    if (addr != nullptr && connection_details_config != nullptr) {
        is_ready_to_connect = true;
    }
}

/**
 * @brief Destructor function
 */
MahdaModbusSlave::~MahdaModbusSlave() {
    disconnect();
}

/**
 * @brief If <is_ready_to_connect> is true, then you can call this function in order to connect the slave node to
 *        master node.
 * @return bool : If the connection has been established then it will return true, otherwise it will return false.
 */
bool MahdaModbusSlave::connect() {
    if (!is_ready_to_connect) {
        return false;
    }
    if (is_connected_to_master) {
        return true;
    }

    if (connection_details.type == TCP) {
        ctx = modbus_new_tcp(connection_details.tcp.ip.c_str(), int(connection_details.tcp.port));
        query = new uint8_t[MODBUS_TCP_MAX_ADU_LENGTH];
    } else if (connection_details.type == TCP_PI) {
        ctx = modbus_new_tcp_pi(connection_details.tcp_pi.node, connection_details.tcp_pi.service);
        query = new uint8_t[MODBUS_TCP_MAX_ADU_LENGTH];
    } else {
        ctx = modbus_new_rtu(connection_details.rtu.device,
                             int(connection_details.rtu.baudrate),
                             connection_details.rtu.parity,
                             connection_details.rtu.data_bit,
                             connection_details.rtu.stop_bit);
        modbus_set_slave(ctx, addresses.server_id);
        query = new uint8_t[MODBUS_RTU_MAX_ADU_LENGTH];
    }
    int header_length = modbus_get_header_length(ctx);

    modbus_set_debug(ctx, TRUE);

    allocate_memory();

    if (connection_details.type == TCP) {
        s = modbus_tcp_listen(ctx, 1);
        modbus_tcp_accept(ctx, &s);
    } else if (connection_details.type == TCP_PI) {
        s = modbus_tcp_pi_listen(ctx, 1);
        modbus_tcp_pi_accept(ctx, &s);
    } else {
        rc = modbus_connect(ctx);
        if (rc == -1) {
            fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
            modbus_free(ctx);
            return false;
        }
    }
    update_memory_from_database();
    is_connected_to_master = true;
    return true;
}

/**
 * @brief Disconnects the slave node from the master node.
 */
void MahdaModbusSlave::disconnect() {
    printf("Quit the loop: %s\n", modbus_strerror(errno));

    if (connection_details.type == TCP) {
        if (s != -1) {
            close(s);
        }
    }

    modbus_mapping_free(mb_mapping);
//    free(query);
    delete query;
    /* For RTU */
    modbus_close(ctx);
    modbus_free(ctx);
    printf("disconnected \n");
    is_connected_to_master = false;
}

/**
 * @brief This will try to process the incoming data_base
 * @return
 */
bool MahdaModbusSlave::listen_for_a_new_request() {
//    while(true){
//        connect();
//    }


//    for (;;) {
    printf("process one \n");
    if (process_one_request()) {
//            is_connected_to_master = false;
        return true;
    }
    return false;
//    }


//    disconnect();
}


uint8_t MahdaModbusSlave::get_tab_bits_by_index(int index) {
    if (index < mb_mapping->nb_bits && index >= 0) {
        return mb_mapping->tab_bits[index];
    }
    return 0;
}

uint8_t MahdaModbusSlave::get_tab_input_bits_by_index(int index) {
    if (index < mb_mapping->nb_input_bits && index >= 0) {
        return mb_mapping->tab_input_bits[index];
    }
    return 0;
}

uint16_t MahdaModbusSlave::get_tab_input_registers_by_index(int index) {
    if (index < mb_mapping->nb_input_registers && index >= 0) {
        return mb_mapping->tab_input_registers[index];
    }
    return 0;
}

uint16_t MahdaModbusSlave::get_tab_registers_by_index(int index) {
    if (index < mb_mapping->nb_registers && index >= 0) {
        return mb_mapping->tab_registers[index];
    }
    return 0;
}

bool MahdaModbusSlave::set_tab_bits_by_index(int index, uint8_t data) {
    if (index < mb_mapping->nb_bits && index >= 0) {
        mb_mapping->tab_bits[index] = data;
        return true;
    }
    return false;
}

bool MahdaModbusSlave::set_tab_input_bits_by_index(int index, uint8_t data) {
    if (index < mb_mapping->nb_input_bits && index >= 0) {
        mb_mapping->tab_input_bits[index] = data;
        return true;
    }
    return false;
}

bool MahdaModbusSlave::set_tab_input_registers_by_index(int index, uint16_t data) {
    if (index < mb_mapping->nb_input_registers && index >= 0) {
        mb_mapping->tab_input_registers[index] = data;
        return true;
    }
    return false;
}

bool MahdaModbusSlave::set_tab_registers_by_index(int index, uint16_t data) {
    if (index < mb_mapping->nb_registers && index >= 0) {
        mb_mapping->tab_registers[index] = data;
        return true;
    }
    return false;
}

float MahdaModbusSlave::get_float_from_input_registers_by_index(int index) {
    conv_float_uint16 union_for_conv{};
    union_for_conv.uint16_arr[0] = get_tab_input_registers_by_index(index);
    union_for_conv.uint16_arr[1] = get_tab_input_registers_by_index(index + 1);
    return union_for_conv.f_number;
}

bool MahdaModbusSlave::set_float_on_input_registers_by_index(int index, float data) {
    conv_float_uint16 union_for_conv{};
    union_for_conv.f_number = data;
    if (set_tab_input_registers_by_index(index, union_for_conv.uint16_arr[0])) {
        if (set_tab_input_registers_by_index(index + 1, union_for_conv.uint16_arr[1])) {
            return true;
        }
    }
    return false;
}

float MahdaModbusSlave::get_float_from_registers_by_index(int index) {
    conv_float_uint16 union_for_conv{};
    union_for_conv.uint16_arr[0] = get_tab_registers_by_index(index);
    union_for_conv.uint16_arr[1] = get_tab_registers_by_index(index + 1);
    return union_for_conv.f_number;
}

bool MahdaModbusSlave::set_float_on_registers_by_index(int index, float data) {
    conv_float_uint16 union_for_conv{};
    union_for_conv.f_number = data;
    if (set_tab_registers_by_index(index, union_for_conv.uint16_arr[0])) {
        if (set_tab_registers_by_index(index + 1, union_for_conv.uint16_arr[1])) {
            return true;
        }
    }
    return false;
}

uint8_t MahdaModbusSlave::get_tab_bits_by_address(uint16_t addr) {
    return get_tab_bits_by_index(addr - addresses.ut_bits_address);
}

uint8_t MahdaModbusSlave::get_tab_input_bits_by_address(uint16_t addr) {
    return get_tab_input_bits_by_index(addr - addresses.ut_input_bits_address);
}

uint16_t MahdaModbusSlave::get_tab_input_registers_by_address(uint16_t addr) {
    return get_tab_input_registers_by_index(addr - addresses.ut_input_registers_address);
}

uint16_t MahdaModbusSlave::get_tab_registers_by_address(uint16_t addr) {
    return get_tab_registers_by_index(addr - addresses.ut_registers_address);
}

bool MahdaModbusSlave::set_tab_bits_by_address(uint16_t addr, uint8_t data) {
    return set_tab_bits_by_index(addr - addresses.ut_bits_address, data);
}

bool MahdaModbusSlave::set_tab_input_bits_by_address(uint16_t addr, uint8_t data) {
    return set_tab_input_bits_by_index(addr - addresses.ut_input_bits_address, data);
}

bool MahdaModbusSlave::set_tab_input_registers_by_address(uint16_t addr, uint16_t data) {
    return set_tab_input_registers_by_index(addr - addresses.ut_input_registers_address, data);
}

bool MahdaModbusSlave::set_tab_registers_by_address(uint16_t addr, uint16_t data) {
    return set_tab_registers_by_index(addr - addresses.ut_registers_address, data);
}

float MahdaModbusSlave::get_float_from_input_registers_by_address(uint16_t start_addr) {
    return get_float_from_input_registers_by_index(start_addr - addresses.ut_input_registers_address);
}

bool MahdaModbusSlave::set_float_on_input_registers_by_address(uint16_t start_addr, float data) {
    return set_float_on_input_registers_by_index(start_addr - addresses.ut_input_registers_address, data);
}

float MahdaModbusSlave::get_float_from_registers_by_address(uint16_t start_addr) {
    return get_float_from_registers_by_index(start_addr - addresses.ut_registers_address);
}

bool MahdaModbusSlave::set_float_on_registers_by_address(uint16_t start_addr, float data) {
    return set_float_on_registers_by_index(start_addr - addresses.ut_registers_address, data);
}

