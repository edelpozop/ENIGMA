#include "simulation.h"

void test_all(char *file)
{
	int argc, i, j;
	char str[50];
	msg_process_t p;

	MSG_create_environment(file);
	MSG_function_register("iot", iot);
	MSG_function_register("dispatcher", dispatcher);
	MSG_function_register("datacenter", datacenter);
	MSG_function_register("dispatcherDatacenter", dispatcherDatacenter);


	int request_data, num_tasks, percentage, num_datacenters, output_data;

	//Datacenter 0

	i = 0;

	for(j = 0; j < 200; j++)
	{
		sprintf(str, "s-0-%d", j);
		argc = 2;
		char **argvc = xbt_new(char *, 3);

		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",j);
		argvc[2] = NULL;

		p = MSG_process_create_with_arguments(str, datacenter, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... s-%d-%d", i, j);
printf("\n");
			exit(0);
		}
	}

	//Datacenter 1

	i = 1;

	for(j = 0; j < 2048; j++)
	{
		sprintf(str, "s-1-%d", j);
		argc = 2;
		char **argvc = xbt_new(char *, 3);

		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",j);
		argvc[2] = NULL;

		p = MSG_process_create_with_arguments(str, datacenter, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... s-%d-%d", i, j);
printf("\n");
			exit(0);
		}
	}

	//Dispatcher Datacenter 0

	i = 0;

	for(j = 0; j < 200; j++)
	{
		sprintf(str, "s-0-%d", j);
		argc = 3;
		char **argvc = xbt_new(char *, 4);

		output_data = 2048;
		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",j);
		argvc[2] = bprintf("%d",output_data);
		argvc[3] = NULL;

		p = MSG_process_create_with_arguments(str, dispatcherDatacenter, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... s-%d-%d", i, j);
printf("\n");
			exit(0);
		}
	}

	//Dispatcher Datacenter 1

	i = 1;

	for(j = 0; j < 2048; j++)
	{
		sprintf(str, "s-1-%d", j);
		argc = 3;
		char **argvc = xbt_new(char *, 4);

		output_data = 1000;
		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",j);
		argvc[2] = bprintf("%d",output_data);
		argvc[3] = NULL;

		p = MSG_process_create_with_arguments(str, dispatcherDatacenter, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... s-%d-%d", i, j);
printf("\n");
			exit(0);
		}
	}

	//IoT Devices from cluster 0

	i = 0;
	request_data = 80;
	num_tasks = 200;
	percentage = 0.9;
	num_datacenters = 2;

	for(j = 0; j < 100; j++)
	{
		sprintf(str, "c-0-%d", j);
		argc = 6;
		char **argvc = xbt_new(char *, 7);

		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",j);
		argvc[2] = bprintf("%d",request_data);
		argvc[3] = bprintf("%d",num_tasks);
		argvc[4] = bprintf("%d",percentage);
		argvc[5] = bprintf("%d",num_datacenters);
		argvc[6] = NULL;

		p = MSG_process_create_with_arguments(str, iot, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... c-%d-%d", i, j);
printf("\n");
			exit(0);
		}
	}

	//IoT Devices from cluster 1

	i = 1;
	request_data = 1024;
	num_tasks = 1000;
	percentage = 0;
	num_datacenters = 2;

	for(j = 0; j < 50; j++)
	{
		sprintf(str, "c-1-%d", j);
		argc = 6;
		char **argvc = xbt_new(char *, 7);

		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",j);
		argvc[2] = bprintf("%d",request_data);
		argvc[3] = bprintf("%d",num_tasks);
		argvc[4] = bprintf("%d",percentage);
		argvc[5] = bprintf("%d",num_datacenters);
		argvc[6] = NULL;

		p = MSG_process_create_with_arguments(str, iot, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... c-%d-%d", i, j);
printf("\n");
			exit(0);
		}
	}

	//IoT Devices from cluster 2

	i = 2;
	request_data = 50;
	num_tasks = 456;
	percentage = 0.5;
	num_datacenters = 2;

	for(j = 0; j < 123; j++)
	{
		sprintf(str, "c-2-%d", j);
		argc = 6;
		char **argvc = xbt_new(char *, 7);

		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",j);
		argvc[2] = bprintf("%d",request_data);
		argvc[3] = bprintf("%d",num_tasks);
		argvc[4] = bprintf("%d",percentage);
		argvc[5] = bprintf("%d",num_datacenters);
		argvc[6] = NULL;

		p = MSG_process_create_with_arguments(str, iot, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... c-%d-%d", i, j);
printf("\n");
			exit(0);
		}
	}

	//Dispatcher 0

	i = 0;

	for(j = 0; j < 2; j++)
	{
		sprintf(str, "d-0-0", j);
		argc = 2;
		char **argvc = xbt_new(char *, 2);

		int nservers = 200;
		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",nservers);
		argvc[2] = NULL;

		p = MSG_process_create_with_arguments(str, dispatcher, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... d-%d-0", i);
printf("\n");
			exit(0);
		}
	}

	//Dispatcher 1

	i = 1;

	for(j = 0; j < 2; j++)
	{
		sprintf(str, "d-1-0", j);
		argc = 2;
		char **argvc = xbt_new(char *, 2);

		int nservers = 2048;
		argvc[0] = bprintf("%d",i);
		argvc[1] = bprintf("%d",nservers);
		argvc[2] = NULL;

		p = MSG_process_create_with_arguments(str, dispatcher, NULL, MSG_get_host_by_name(str), argc, argvc);
		if(p == NULL)
		{
			printf("Error en ......... d-%d-0", i);
printf("\n");
			exit(0);
		}
	}
	return;
}




int main(int argc, *argc[])
{
	msg_error_t res = MSG_OK;
	int i, j;
	double t_medio_servicio = 0.0;
	double q_medio = 0.0;
	double n_medio = 0.0;

	if (argc < 3)
	{
		printf("Usage: %s platform_file lambda", argv[0]);
printf("\n");
		exit(1);
	}


	seed((int)time(NULL));
	ARRIVAL_RATE = atof(argv[2]) * MAX_SERVERS;
	sg_host_enery_plugin_init();
	MSG_init(&argc, argv);




	for(j = 0; j < 200; j++)
	{
		tasksManagement[0].Nqueue[j] = 0;
		tasksManagement[0].Nsystem[j] = 0;
		tasksManagement[0].EmptyQueue[j] = 0;
		tasksManagement[0].mutex[j] = xbt_mutex_init();
		tasksManagement[0].cond[j] = xbt_cond_init();
		tasksManagement[0].client_requests[j] = xbt_dynar_new(sizeof(struct ClientRequest *), NULL);
	}



	for(j = 0; j < 2048; j++)
	{
		tasksManagement[1].Nqueue[j] = 0;
		tasksManagement[1].Nsystem[j] = 0;
		tasksManagement[1].EmptyQueue[j] = 0;
		tasksManagement[1].mutex[j] = xbt_mutex_init();
		tasksManagement[1].cond[j] = xbt_cond_init();
		tasksManagement[1].client_requests[j] = xbt_dynar_new(sizeof(struct ClientRequest *), NULL);
	}

	test_all(argv[1]);
	res = MSG_main();
	int totaltasks = 0;



	i = 0;
	for(j = 0; j < 200; j++)
	{
		q_medio = q_medio + taskManagement[0].Navqueue[j];
		n_medio = n_medio + taskManagement[0].Navsystem[j];
	}
printf("DATACENTER \t tiempoMedioServicio \t TamañoMediocola \t    TareasMediasEnElSistema  \t   tareas\n");
printf("%i \t\t %g \t\t\t %g \t\t\t  %g  \t\t\t  %d \n\n", i, t_medio_servicio, q_medio, n_medio, avServTime[i].numTasks);
	t_medio_servicio = 0;
	q_medio = 0;
	n_medio = 0;
	totaltasks += avServTime[i].numTasks;



	i = 1;
	for(j = 0; j < 2048; j++)
	{
		q_medio = q_medio + taskManagement[1].Navqueue[j];
		n_medio = n_medio + taskManagement[1].Navsystem[j];
	}
printf("DATACENTER \t tiempoMedioServicio \t TamañoMediocola \t    TareasMediasEnElSistema  \t   tareas\n");
printf("%i \t\t %g \t\t\t %g \t\t\t  %g  \t\t\t  %d \n\n", i, t_medio_servicio, q_medio, n_medio, avServTime[i].numTasks);
	t_medio_servicio = 0;
	q_medio = 0;
	n_medio = 0;
	totaltasks += avServTime[i].numTasks;

\tprintf("Simulation time %g\n", MSG_get_clock());



	for(j = 0; j < 200; j++)
	{
		xbt_dynar_free(&taskManagement[0].client_requests[j]);
	}



	for(j = 0; j < 2048; j++)
	{
		xbt_dynar_free(&taskManagement[1].client_requests[j]);
	}


	if (res == MSG_OK) return 0;
	else return 1;
}
