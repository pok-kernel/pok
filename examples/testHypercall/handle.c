
#define PIT_IRQ 0
uint64_t pok_tick_counter = 0;
pok_ret_t handle_irq(uint8_t irq)
{
  switch(irq)
  {
case PIT_IRQ:
    pok_tick_counter ++;
    printf( "Clock gettick: %u \n",pok_tick_counter);
    
}
    


