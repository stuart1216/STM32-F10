// limiting amplitude wave filter
#define A 10 // can be set by need
char value;  // for 8 bit ADC only, can be set as 'bit'
char laf()
{
        char new_value;
        new_value = get_ad();
        if ( ( new_value - value > A ) || ( value - new_value > A )
        return value;
        return new_value;
}
