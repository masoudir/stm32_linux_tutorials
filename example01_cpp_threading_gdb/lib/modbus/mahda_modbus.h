
#ifndef MAHDA_MODBUS_H
#define MAHDA_MODBUS_H

#include <iostream>

#ifdef _WIN32
# include <winsock2.h>
#else

# include <sys/socket.h>

#endif

/* For MinGW */
#ifndef MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif



/* Constants defined by configure.ac */
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1

#ifdef HAVE_INTTYPES_H

#include <inttypes.h>

#endif
#ifdef HAVE_STDINT_H
# ifndef _MSC_VER

# include <stdint.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# else
# include "stdint.h"
# endif
#endif

#define TCP_IP_CHAR_MAX_SIZE 15


#define BUG_REPORT(_cond, _format, _args ...) \
    printf("\nLine %d: assertion error for '%s': " _format "\n", __LINE__, # _cond, ## _args)

#define ASSERT_TRUE(_cond, _format, __args...)   \
    if (_cond) {                                  \
        printf("OK\n");                           \
    } else {                                      \
        BUG_REPORT(_cond, _format, ## __args);    \
    }                                             \


/**
 * @brief The supported types of connections in Modbus
 */
enum modbus_type {
    MODBUS_DISABLED, /* No connection */
    TCP,        /* TCP connection. Supports multi-slave modbus nodes. */
    TCP_PI,     /* TCP-PI connection */
    RTU         /* RTU connection (Serial port) */
};

/**
 * @brief The details of TCP connection used for Modbus
 */
struct modbus_tcp_connection_details {
    std::string ip;     /* Stores the ip address. (e.g. "192.168.1.84") */
    unsigned long port; /* Stores the port number */
};

/**
 * @brief The details of TCP-PI connection used for Modbus
 */
struct modbus_tcp_pi_connection_details {
    const char *node;       /* Stores the pointer to node name */
    const char *service;    /* Stores the pointer to service name */
};

/**
 * @brief The details of RTU connection used for Modbus
 */
struct modbus_rtu_connection_details {
    const char *device;         /* Device name (e.g. "/dev/ttyS0") */
    unsigned long baudrate;     /* Baudrate (default=115200) */
    char parity;                /* Parity (default='N') */
    unsigned short data_bit;    /* Data size in terms of number of bits (default=8) */
    unsigned short stop_bit;    /* Stop bits size (default=1) */
};

/**
 * @brief The details of each connection types used for Modbus
 */
struct modbus_connection_details {
    modbus_type type;                           /* Specifies which type of connection are going to be used. */

    modbus_tcp_connection_details tcp;          /* If the type is being set to TCP, then it would store all the
                                                * TCP connection details. */

    modbus_tcp_pi_connection_details tcp_pi{};  /* If the type is being set to TCP-PI, then it would store all the
                                                * TCP-PI connection details. */

    modbus_rtu_connection_details rtu{};        /* If the type is being set to RTU, then it would store all the
                                                * RTU connection details. */
};

/**
 * @brief The details of the addresses used for Modbus
 */
struct ModbusAddresses {
    uint16_t server_id;                    /* The ID of server (slave) in the Modbus network */                             //17

    uint16_t ut_bits_address;              /* Starting address of Readable-Writable bits section in the Modbus network */   //0x0

    uint16_t ut_bits_nb;                   /* The number of bits used for the section of Readable-Writable bits */          //0x25

    uint16_t ut_input_bits_address;        /* Starting address of Readable (input) bits section in the Modbus network
                                            * (Only slave can change the contents of the bits in this section) */           //0x1c4

    uint16_t ut_input_bits_nb;             /* The number of bits used in Readable (input) bits section */                   //0x16

    uint16_t ut_registers_address;         /* Starting address of Readable-Writable registers section in the Modbus
                                            * network (each register contains two bytes) */                                 //0x160

    uint16_t ut_registers_nb;              /* The number of Readable-Writable registers section in the Modbus
                                            * network (each register contains two bytes) */                                 //0x3

    uint16_t ut_registers_nb_max;          /* The maximum number of Readable-Writable registers section in the Modbus
                                            * network (used for creating arrays in the code and must be equal or bigger
                                            * than ut_registers_nb) */                                                      //0x20

    uint16_t ut_input_registers_address;   /* Starting address of Readable registers section in the Modbus network
                                            * (each register contains two bytes)
                                            * (Only slave can change the contents of the registers in this section) */      //0x108                                       //0x108

    uint16_t ut_input_registres_nb;        /* The number of Readable registers section in the Modbus
                                            * network (each register contains two bytes) */                                 //0x1

};

/**
 * @brief Used for converting float32 data_base to array of unsigned int data_base with the size of two and vice versa.
 */
union conv_float_uint16 {
    float f_number;                         /* Float32 data_base */
    uint16_t uint16_arr[2];                 /* An array of 16 bits unsigned integer with a size of two */
};


#endif //MAHDA_MODBUS_H
