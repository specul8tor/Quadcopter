#include <stdio.h>
#include "A7125Radio.h"
#include "A7125RadioData.h"
#include "BaseBand.h"
#include "IRadio.h"
#include "WInStream.h"
#include "WOutStream.h"

uint16_t first;
static const uint16_t baud_rate = 21844;
static const uint16_t start_word = 0xCCCC;



int main(){

    //establish BBU class (it is a )
    BaseBandUnit<BBU0> BBU(
        uint16_t brg, // replace with appropriate value
        uint16_t cfg0 = BBU_enable | , 
        uint16_t start_word = 0, 
        uint16_t cfg1 = 0
        );

    // cfg0 depending on what I put in it will be put into the bb0 register check user guide
    // BBU.h we can or the bits
    // baud rate target is 2.048 mb/second // use formula in user guide

    using radioSPI = SerialPortInterface<SPI1>;
    radioSPI spi;
    //SPI1 is what the ginseng will use to connect to radio

    // port for BBUoutput
    using PortE = spine::Port<GPIO_E, 0x100, 0x1>;
    decltype(PortE::CreateOutput<0>()) radio_cs;

    // port for pin that controls direction of BB0O (because its bidirectional)
    using PortH = spine::Port<GPIO_H, 0x400, 0x4>;
    decltype(PortH::CreateOutput<2>()) radio_bbu_direction;


    // define SPI object
    SpiWithChipSelect<decltype(spi), decltype(radio_cs)>  spi_with_cs(spi, radio_cs);

    //define a radio object
    GinsengPAControl pa_control;

    A7125Radio<
        GinsengPAControl,
        SpiWithChipSelect,
        BaseBandUnit<BBU0>,
        PortE,
        decltype(radio_bbu_dir)
    > radio(
        pa_control, spi_with_cs, BBU, radio_bbu_direction
    );

    //define the baseband
    //establish a clock type

    DummyCrc fec;
    DummyCrc crc;

    BaseBand<BaseBandUnit<BBU0>> base_band(BBU, start_word);
    std::array<uint16_t, 20> data;
    while (true) {

        radio.SwitchToReceive();

        if (auto win = base_band.TryAcquireInputStream(timer, crc, fec)) {
            //deal with the win
            for (uint16_t i = 0; i < 20; i++) {
                data[i] = win.GetWordWithNoCrcOrFec();
            }
        }
    
        wait_1ms();

        radio.SwitchToTransmit(0);

        if (auto wout = base_band.ForceAcquireOutputStream()) {
            for (auto& elem: data_array) {
                wout.PushWordWithoutEncoding(elem);
            }
        }

    }

    return 0;
}