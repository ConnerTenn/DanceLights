
#include <iostream>
#include <ws2811.h>

ws2811_led_t strip[300];

bool Run = true;

static void ctrl_c_handler(int signum)
{
	(void)(signum);
	Run = false;
}

void setup_handlers()
{
	sigaction sa = { .sa_handler = ctrl_c_handler };
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

int main()
{
	ws2811_return_t ret;
	
	
	
	setup_handlers();
	
	if((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
	{
		std::cout << "ERROR::ws2811 init failed: " << ws2811_get_return_t_str(ret) << "\n";
		return ret;
	}
	
	while (Run)
	{
		
	}

	return 0;
}
