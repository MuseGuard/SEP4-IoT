#include "monitoring_system_control.h"
#include "measurements_controller.h"
#include"package_builder.h"
#include "connection_controller.h"
#include "pc_comm.h"
#include <stdlib.h>

void monitoring_system_control_execute(){
    char *temperature_and_humidity = measurements_controller_get_temperature_and_humididty();
    char *light_levels = measurements_controller_get_lightlevels();
    Package package = package_builder_build(temperature_and_humidity, light_levels);
    free(temperature_and_humidity);
    free(light_levels);
    pc_comm_send_string_blocking(package.data);
    connection_controller_transmit(package);
};
    
    
void monitoring_system_change_state(char sensor, char state){

};