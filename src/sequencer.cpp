#include "sequencer.h"
#include "vasynth.h"

#include "daisy_seed.h"

// Set sequencer memory to SRAM
#define WRITE_READ_SRAM_ADDR 0x20000000

/* Process sequencer requests */
static void callback(void *data)
{
    static_cast<Sequencer *>(data)->play();
}

/* SRAM memory handling routines */
void writeSram(uint32_t l_addr, uint8_t l_data)
{
    /* Pointer write on specific location of backup SRAM */
    *(volatile uint8_t *)(WRITE_READ_SRAM_ADDR + l_addr) = l_data;
}

uint8_t readSram(uint32_t l_addr)
{
    uint8_t i_retval;

    /* Pointer write from specific location of backup SRAM */
    i_retval = *(volatile uint8_t *)(WRITE_READ_SRAM_ADDR + l_addr);

    return i_retval;
}

void writeSramWord(uint32_t l_addr_w, uint16_t l_data_w)
{
    /* Pointer write on specific location of backup SRAM */
    *(volatile uint16_t *)(WRITE_READ_SRAM_ADDR + l_addr_w) = l_data_w;
}

uint16_t readSramWord(uint32_t l_addr_w)
{
    uint16_t i_retval_w;

    /* Pointer write from specific location of backup SRAM */
    i_retval_w = *(volatile uint16_t *)(WRITE_READ_SRAM_ADDR + l_addr_w);

    return i_retval_w;
}

Sequencer::Sequencer(std::shared_ptr<VASynth> va_synth)
: clock_{0},
  time_{0},
  mode_{Mode::Stop},
  memory_{0x00010000},
  vasynth_{va_synth}
{
    /* Create Timer Handle and Config */
    daisy::TimerHandle         tim5;
    daisy::TimerHandle::Config tim_cfg;

    /** TIM5 with IRQ enabled */
    tim_cfg.periph     = daisy::TimerHandle::Config::Peripheral::TIM_5;
    tim_cfg.enable_irq = true;

    /** Configure frequency (240Hz) */
    auto tim_target_freq = 240;
    auto tim_base_freq   = daisy::System::GetPClk2Freq();
    tim_cfg.period       = tim_base_freq / tim_target_freq;

    /** Initialize timer */
    tim5.Init(tim_cfg);
    tim5.SetCallback(callback, this);

    /** Start the timer, and generate callbacks at the end of each period */
    tim5.Start();
}

void Sequencer::reset()
{
    clock_  = 0;
    memory_ = memory_origin_;
}

void Sequencer::play()
{
    if(mode_ == Mode::Stop)
    {
        reset();
        return;
    }

    if(mode_ == Mode::Play)
    {
        /* Read the time-stamp from SRAM and compare to seqclock*/
        time_ = readSramWord(memory_);

        while(time_ == clock_)
        {
            /* Move pointer to note data */
            memory_++;
            memory_++;

            /* Read the note from SRAM */
            seqnote = readSram(memory_);
            seqmsg  = seqnote & 0x80;
            seqnote = seqnote & 0x7F;
            memory_++;

            /* Read the velocity from SRAM */
            seqvelocity = readSram(memory_);
            memory_++;

            if(seqvelocity == 0xFF)
            {
                /* End of sequence reached, reset the position to the beginning and restart */
                reset();
                return;
            }

            if(seqmsg)
            {
                /* Play the note */
                vasynth_->NoteOn(seqnote, seqvelocity);
            }
            else
            {
                /* Stop the note */
                vasynth_->NoteOff(seqnote);
            }

            /* Check to see if there are other events at the current sequencer clock */
            time_ = readSramWord(memory_);
        }
    }

    /* All events processed so increment the sequencer clock */
    clock_++;
}

void Sequencer::record(uint8_t recnote, uint8_t recvelocity)
{
    /* Write the timestamp to SRAM */
    writeSramWord(memory_, clock_);
    memory_++;
    memory_++;

    /* Write the note to SRAM */
    writeSram(memory_, recnote);
    memory_++;

    /* Write the velocity to SRAM */
    writeSram(memory_, recvelocity);
    memory_++;

    if(memory_ > 0x7FFF8)
    {
        /* There is no more memory left for the sequencer to use */
        /* Place a timestamp and stop marker at the current position and select stop mode */
        writeSramWord(memory_, clock_);
        memory_++;
        memory_++;
        writeSramWord(memory_, 0xFFFF);
        stop_mode();
    }
}

void Sequencer::record_mode(bool enable)
{
    if(enable)
    {
        // Sequencer Mode Record
        mode_  = Mode::RecordEnable;
        clock_ = 0;
    }
    else
    {
        mode_ = Mode::RecordDisable;
        // Place a time-stamp and stop marker at the current position and stop the sequencer
        writeSramWord(memory_, clock_);
        memory_++;
        memory_++;
        writeSramWord(memory_, 0xFFFF);
        stop_mode();
    }
}

void Sequencer::stop_mode()
{
    mode_ = Mode::Stop;
    reset();
}

void Sequencer::play_mode()
{
    mode_ = Mode::Play;
    reset();
}
