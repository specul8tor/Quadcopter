//#include <stdio.h>
#include "GPIO.h"
#include "A7125Radio.h"
#include "A7125RadioData.h"
#include "BaseBand.h"
#include "IRadio.h"
#include "WInStream.h"
#include "WOutStream.h"
#include "GinsengPAControl.h"
//#include <chrono>
//#include <thread>
#include <SoftTimer.h>
#include <BBU.h>
#include <SPI.h>
#include <HW_BBU.h>
#include <HW_SPI.h>
#include <Port.h>





using namespace std;

uint16_t first;
static const uint16_t baud_rate = 21844;
static const uint16_t start_word = 0xCCCC;
uint16_t cfg0 = BBU_ENABLE | BBU_TX_ENABLE | BBU_BIDIRECTION_ENABLE | BBU_ROM_ENABLE;
uint16_t cfg1 = 0;

int main(){

    
    //establish BBU class
    BaseBandUnit<BBU0> BBU(baud_rate, cfg0, start_word, cfg1);

    // cfg0 depending on what I put in it will be put into the bb0 register check user guide
    // BBU.h we can or the bits
    // bitrate target is 2.048 Mb/second // use formula in user guide
    // Bit rate = (sys_clk/8) * (1/(65535+1)) *(BBUbrg + 1) 



    using radioSPI = SerialPortInterface<SPI1>;
    radioSPI spi(_49_152_MHz);
    //SPI1 is what the ginseng will use to connect to radio

    // port for BBUoutput
    // GPIO_E = 4
    using PortE = spine::Port<GPIO_E, 0x100, 0x1>;
    decltype(PortE::CreateOutput<0>()) radio_cs;

    // port for pin that controls direction of BB0O (because its bidirectional)
    // GPIO_H = 7
    using PortH = spine::Port<GPIO_H, 0x400, 0x4>;
    decltype(PortH::CreateOutput<2>()) radio_bbu_direction;


    // define SPI object
    SpiWithChipSelect<radioSPI, decltype(radio_cs)>  spi_with_cs(spi, radio_cs);

    //define a radio object
    using PAControl = IPAControl<GinsengPAControl>;
    PAControl pa_control;

    //define configure parameters
    using ConfigParam = StandardA7125ConfigParams;


    A7125Radio<
        ConfigParam,
        GinsengPAControl,
        decltype(spi_with_cs),
        BaseBandUnit<BBU0>,
        PortE,
        decltype(radio_bbu_direction)
    > radio(
        pa_control, spi_with_cs, BBU, radio_bbu_direction
    );

    //define the baseband
    //establish a clock type
    SoftTimer<decltype(BBU)> timer(BBU);
    SoftTimer<decltype(BBU)> poll_cycle_timer(BBU);
    SoftTimer<uint16_t>sending(); // clock constant?

    BaseBand<BaseBandUnit<BBU0>> base_band(BBU, start_word);
    std::array<uint16_t, 20> data;
    uint16_t value = 1000;

    uint16_t LED = 0x0001;

    radio.ChangeChannel(25); // channels go from 0-49

    poll_cycle_timer.Start(0);
    while (true) {
        poll_cycle_timer.Tick(1000);

        // radio.SwitchToReceive();
        // timer.Start(value);

        // if (auto win = base_band.TryAcquireInputStream(timer, EncoderStrategy::None)) {
        //     //deal with the win
        //     for (uint16_t i = 0; i < 20; i++) {
        //         data[i] = win.GetWordWithNoCrcOrFec();
        //     }
        // }

        // timer.Wait();
        // timer.Start(value); // design
        // // wait for a milisecond
        // // baud rate relates to ticks

        // radio.SwitchToTransmit(0);

        // if (auto wout = base_band.TryAcquireOutputStream()) {
        //     for (auto& elem: data) {
        //         wout.PushWordWithoutEncoding(elem);
        //     }
        // }
       // timer.Wait();

        
        //this is for turning on an LED:

        radio.SwitchToTransmit(0);

        if (auto wout = base_band.TryAcquireOutputStream()) {
                wout.PushWordWithoutEncoding(LED);
        }

        
       poll_cycle_timer.Wait();
    }

    return 0;
}