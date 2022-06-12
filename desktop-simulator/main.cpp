#include "scheduler.h"
#include "desktop_hardware_manager.h"

#include "tcp_socket.h"

int main(int argC, char**argv) {

    if(argC >3) {
        output_file = argv[1];
        ext_flash_path = argv[2];
        int_flash_path = argv[3];
        printf("Running with output file %s, ext flash %s, int flash %s\n",
            output_file.c_str(), ext_flash_path.c_str(), int_flash_path.c_str());
    } else {
        output_file = "/mnt/d/Documents/GitHub/pyqt_groundstation/output/LDRS-beanboozler-l265-output-post.csv";
        ext_flash_path = "external_flash.dat";
        int_flash_path = "internal_flash.dat";
        // printf("Must have 3 args! Eg ./desktop_sim path/to/flight.csv external_flash.dat internal_flash.dat\n");
        // return 1;
    }

    Scheduler s = Scheduler();
    s.run();

    return 0;
}