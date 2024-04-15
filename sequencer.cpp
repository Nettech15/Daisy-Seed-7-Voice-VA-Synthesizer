#include "sequencer.h"
#include "vasynth.h"

#include "daisy_seed.h"

// Set sequencer memory to SRAM
#define WRITE_READ_SRAM_ADDR 0x20000000

/* Process sequencer requests */
static void callback(void* data)
{
    Sequencer *pSeq = static_cast<Sequencer *>(data);
	pSeq->play(pSeq->get_mode());
}

/* SRAM memory handling routines */
void writeSram(uint32_t l_addr, uint8_t l_data)
{
  /* Pointer write on specific location of backup SRAM */
  *(volatile uint8_t *) (WRITE_READ_SRAM_ADDR + l_addr) = l_data;
}

uint8_t readSram(uint32_t l_addr)
{
  uint8_t i_retval;
	
  /* Pointer write from specific location of backup SRAM */
  i_retval =  *(volatile uint8_t *) (WRITE_READ_SRAM_ADDR + l_addr);
	
  return i_retval;
}

void writeSramWord(uint32_t l_addr_w, uint16_t l_data_w)
{
  /* Pointer write on specific location of backup SRAM */
  *(volatile uint16_t *) (WRITE_READ_SRAM_ADDR + l_addr_w) = l_data_w;
}

uint16_t readSramWord(uint32_t l_addr_w)
{
  uint16_t i_retval_w;
	
  /* Pointer write from specific location of backup SRAM */
  i_retval_w =  *(volatile uint16_t *) (WRITE_READ_SRAM_ADDR + l_addr_w);
	
  return i_retval_w;
}

Sequencer::Sequencer(std::shared_ptr<VASynth> va_synth) :
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

void Sequencer::play(uint16_t modenum)
{
	if(modenum == 0)
	{
		seqclock = 0;
		seqmem = 0x00010000;
		return;
	}
	
	if(modenum == 3)
	{
		/* Read the time-stamp from SRAM and compare to seqclock*/
		seqtime = readSramWord(seqmem);
		
		while(seqtime == seqclock)
		{
			/* Move pointer to note data */
			seqmem++;
			seqmem++;
			
			/* Read the note from SRAM */
			seqnote = readSram(seqmem);
			seqmsg = seqnote & 0x80;
			seqnote = seqnote & 0x7F;
			seqmem++;
		
			/* Read the velocity from SRAM */
			seqvelocity = readSram(seqmem);
			seqmem++;
			
			if(seqvelocity == 0xFF)
			{
				/* End of sequence reached, reset the position to the beginning and restart */
				seqclock = 0;
				seqmem = 0x00010000;
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
			seqtime = readSramWord(seqmem);
		}
	}
	
	/* All events processed so increment the sequencer clock */
	seqclock++;
}

void Sequencer::record(uint8_t recnote, uint8_t recvelocity)
{
	/* Write the timestamp to SRAM */
	writeSramWord(seqmem, seqclock);
	seqmem++;
	seqmem++;
		
	/* Write the note to SRAM */
	writeSram(seqmem, recnote);
	seqmem++;
		
	/* Write the velocity to SRAM */
	writeSram(seqmem, recvelocity);
	seqmem++;
	
	if(seqmem > 0x7FFF8)
	{
		/* There is no more memory left for the sequencer to use */
		/* Place a timestamp and stop marker at the current position and select stop mode */
		writeSramWord(seqmem, seqclock);
		seqmem++;
		seqmem++;
		writeSramWord(seqmem, 0xFFFF);
		seqmode = 0;
		seqclock = 0;
		seqmem = 0x00010000;
	}
}

void Sequencer::record_mode(bool enable)
{
    if (enable)
    {
        // Sequencer Mode Record
        seqmode = 1;
        seqclock = 0;
    }
    else
    {
        seqmode = 2;
        // Place a time-stamp and stop marker at the current position and stop the sequencer
        writeSramWord(seqmem, seqclock);
        seqmem++;
        seqmem++;
        writeSramWord(seqmem, 0xFFFF);
        seqmode = 0;
        seqclock = 0;
        seqmem = 0x00010000;
    }
}

void Sequencer::stop_mode()
{
    seqmode = 0;
    seqclock = 0;
    seqmem = 0x00010000;
}

void Sequencer::play_mode()
{
    seqmode = 3;
    seqclock = 0;
    seqmem = 0x00010000;
}

