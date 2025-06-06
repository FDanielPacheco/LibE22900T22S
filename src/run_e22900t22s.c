#include <mixip.h> 
#include <e22900t22s.h>
#include <time.h>

#ifdef RPI4
  #define CHIPN  "gpiochip0"
  #define AUX    4
  #define M0     22
  #define M1     27
#elif XU4
  #define CHIPN  "gpiochip2"
  #define AUX    2
  #define M0     6
  #define M1     5
#else
  #define CHIPN  ""
  #define AUX    0
  #define M0     0
  #define M1     0
#endif

e22900t22s_t driver;

/**************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
void
printPacket( const uint8_t * pk, const size_t len, const char * cover, int columnsize ){
  if( !pk ) return;

  time_t t;
  struct tm * info;
  time( &t );
  info = localtime( &t );
  char tm[12]; strftime( tm, sizeof(tm), "%H:%M:%S", info );

  int cont = columnsize;
  printf("<-----%s Packet [%s]----->\n", cover, tm );
  for( size_t i = 0 ; i < len ; ++i ){
    if( !cont || (i + 1 == len) ){
      printf( "%04zu:%02X\n", i, pk[i] );
      cont = columnsize;
    }
    else {
      printf( "%04zu:%02X ", i, pk[i] );
      cont--;
    }
  }
  printf("<-----%s Packet----->\n\n", cover);
}

/**************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
int dsetup( serial_manager_t * serial ){
  driver.serial = serial;

  int8_t ret = e22900t22s_gpio_init( CHIPN, M0, M1, AUX, &driver );
  if( -1 == ret )
    return -1;
  
  ret = e22900t22s_get_config( &driver );
  if( -1 == ret ){
    e22900t22s_gpio_close( &driver );
    return -1;
  }

  // (void) e22900t22s_print_config( 1, &driver );

  // Configurations
  e22900t22s_default_config( 0, &driver );
  e22900t22s_set_baudrate( B38400, &driver );
  e22900t22s_set_airrate( B38400, &driver );
  e22900t22s_set_packet_size( E22900T22S_PACKET_32, &driver );
  e22900t22s_set_listen_before_talk( 1, &driver );

  ret = e22900t22s_update_eeprom( &driver );
  if( -1 == ret ){
    e22900t22s_gpio_close( &driver );
    return -1;
  }

  ret = e22900t22s_get_config( &driver );
  if( -1 == ret ){
    e22900t22s_gpio_close( &driver );
    return -1;
  }

  (void) e22900t22s_print_config( 1, &driver );
  printf("[%d] Device configured...\n", getpid( ) );
  return 0; 
}
 
/**************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
int dloop( flow_t * flow ){
  // Runs in loop, in a separeted process, consider limiting the CPU poll with a sleep...
  // To stop the other process (read/write) use halt_network( flow ), and continue_network( flow )
  
  return 0; 
}
 
/**************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
int dread( buffer_t * buf ){
  
  return 0; 
}
 
/**************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
int dwrite( buffer_t * buf ){
  
  return 0; 
}
