#include "adc_interface.h"

#include <libdigiapix/adc.h>

int read_adc(int chip, int channel)
{
	adc_t *adc;
	int output;

	adc = ldx_adc_request(chip,channel);
	output = ldx_adc_get_sample(adc);
	ldx_adc_free(adc);

	return output;

}
