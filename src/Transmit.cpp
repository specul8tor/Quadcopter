#include <stdio.h>
#include "A7125Radio.h"
#include "A7125RadioData.h"
#include "BaseBand.h"
#include "IRadio.h"
#include "WInStream.h"
#include "WOutStream.h"
#include <chrono>
#include <thread>

using namespace std;

uint16_t first;
static const uint16_t baud_rate = 21844;
static const uint16_t start_word = 0xCCCC;


int main(){

    //establish BBU class (it is a )
    BaseBandUnit<BBU0> BBU(
        uint16_t baud_rate, // replace with appropriate value
        uint16_t cfg0 = BBU_enable | BBU_TX_ENABLE | BBU_USE_CLK_IN | BBU_CLK_DETECT_ENABLE | BBU_CLK_ENABLE | BBU_CLK_IN_RISING_EDGE | BBU_TX_MODE_RET_TO_ZERO | BBU_BIDIRECTION_ENABLE | BBU_EXT_CLK_TO_INC,
        uint16_t start_word, 
        uint16_t cfg1 = 0,
        );

    // cfg0 depending on what I put in it will be put into the bb0 register check user guide
    // BBU.h we can or the bits
    // baud rate target is 2.048 mb/second // use formula in user guide
    // Bit rate = (sys_clk/8) * (1/(65535+1)) *(BBUbrg + 1) 

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
    using PAControl = IPAControl<PAControlTy>;
    PAControl pa_control;

    //define configure parameters
    using ConfigParam = StandardA7125ConfigParams;


    A7125Radio<
        ConfigParam,
        PAControl,
        SpiWithChipSelect,
        BaseBandUnit<BBU0>,
        PortE,
        decltype(radio_bbu_direction)
    > radio(
        pa_control, spi_with_cs, BBU, radio_bbu_direction
    );

    //define the baseband
    //establish a clock type

    EncoderStrategy Edata;

    Edata.Configure = 1;
    Edata.Reset = 4;
    // what are the crc
    // forward error correction

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
    
        this_thread::sleep_for(std::chrono::milliseconds(1)); // wait for a milisecond

        radio.SwitchToTransmit(0);

        if (auto wout = base_band.TryAcquireOutputStream()) {
            for (auto& elem: data) {
                wout.PushWordWithoutEncoding(elem);
            }
        }

    }

    return 0;
}