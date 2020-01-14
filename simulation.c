#include "headers_platform.h"

// Sort two elements of type struct ClientRequest. It uses the function xbt_dynar_sort
static int sort_function_short(const void *e1, const void *e2)
{
	struct ClientRequest *c1 = *(void **)e1;
	struct ClientRequest *c2 = *(void **)e2;

	if (c1->t_service == c2->t_service)
		return 0;
	else if (c1->t_service < c2->t_service)
		return -1;
	else
		return 1;
}



// Client function: Creates the requests
int client(int argc, char *argv[])
{
	double task_comp_size = 0;
	double task_comm_size = 0;
	char sprintf_buffer[64];
	char mailbox[256], buf[64];
	msg_task_t task = NULL;
	struct ClientRequest *req;
	struct ServerResponse *resServer;
	double t_arrival;
	int my_iot_cluster, my_device, dispatcher;
	double t;
	int res, k;

	my_iot_cluster = atoi(argv[0]);
	my_device = atoi(argv[1]);

	sprintf(buf, "c-%d-%d", my_iot_cluster,my_device);
	MSG_mailbox_set_async(buf); //mailbox asincrono

	msg_host_t host = MSG_host_by_name(buf);
	
	if (percentageTasks == 1)								//If percentageTasks is equal to 1 then the tasks execute on the devices
	{
		for (k = 0; k < NUM_TASKS; k++)
		{
			sprintf(sprintf_buffer, "Task_%d_%d_%d", my_iot_cluster, my_device, k);
			double start = MSG_get_clock();
			msg_task_t taskLocally = NULL;
			double serviceLocally = MFLOPS_BASE * exponential((double)SERVICE_RATE) * percentageTasks;
			taskLocally = MSG_task_create(sprintf_buffer, serviceLocally, SIZE_DATA, NULL);
			MSG_task_execute(taskLocally);
			MSG_task_destroy(taskLocally);

			printf("Task done in %s (duration: %.2f s). Current peak speed=%.0E flop/s; Current consumption: from %.0fW to %.0fW"
			" depending on load; Energy dissipated=%.0f J\n\n",
			MSG_host_get_name(host), MSG_get_clock() - start, MSG_host_get_speed(host), sg_host_get_wattmin_at(host, MSG_host_get_pstate(host)),
			sg_host_get_wattmax_at(host, MSG_host_get_pstate(host)), sg_host_get_consumed_energy(host));
		}
	}
	else 													//If not, the devices create the requests that execute the datacenters 
	{
		for (k = 0; k < NUM_TASKS; k++)
		{
			req = (struct ClientRequest *)malloc(sizeof(struct ClientRequest));

			t_arrival = exponential((double)ARRIVAL_RATE);
			MSG_process_sleep(t_arrival);

			sprintf(sprintf_buffer, "Task_%d_%d_%d", my_iot_cluster, my_device, k);
			sprintf(req->id_task, "%s", sprintf_buffer);

			req->t_arrival = MSG_get_clock(); // tiempo de llegada

			req->iot_cluster = my_iot_cluster;
			req->device = my_device;

			t = exponential((double)SERVICE_RATE);

			req->t_service = MFLOPS_BASE * t; 			// calculo del tiempo de servicio en funcion
											  			// de la velocidad del host del servidor

			if (percentageTasks != 0)					//The devices compute locally part of the tasks
			{
				double start = MSG_get_clock();
				msg_task_t taskLocally = NULL;
				double serviceLocally = MFLOPS_BASE * t * percentageTasks;
				taskLocally = MSG_task_create(sprintf_buffer, serviceLocally, SIZE_DATA, NULL);
				MSG_task_execute(taskLocally);
				MSG_task_destroy(taskLocally);
				req->t_service = req->t_service - serviceLocally;
				
				printf("Task partially done in %s (duration: %.2f s). Current peak speed=%.0E flop/s; Current consumption: from %.0fW to %.0fW"
				" depending on load; Energy dissipated=%.0f J\n\n",
				MSG_host_get_name(host), MSG_get_clock() - start, MSG_host_get_speed(host), sg_host_get_wattmin_at(host, MSG_host_get_pstate(host)),
				sg_host_get_wattmax_at(host, MSG_host_get_pstate(host)), sg_host_get_consumed_energy(host));

			}

			req->n_task = k;
			task_comp_size = req->t_service;
			task_comm_size = 0;

			task = MSG_task_create(sprintf_buffer, task_comp_size, task_comm_size, NULL);
			MSG_task_set_data(task, (void *)req);

			dispatcher = uniform_int(0, NUM_DATACENTERS-1);
			sprintf(mailbox, "d-%d-0", dispatcher);
			MSG_task_send(task, mailbox);
			task = NULL;
		}

		while(1)
		{
			res = MSG_task_receive_with_timeout(&(task), MSG_host_get_name(MSG_host_self()), MAX_TIMEOUT_SERVER);
			if (res != MSG_OK) break;
			resServer = MSG_task_get_data(task);
			//printf("%s completed on server %d-%d\n",resServer->id_task, resServer->server_cluster, resServer->server);
			
			free(resServer);
			MSG_task_destroy(task);
			task = NULL;
		}

	}

	/* finalizar */
	return 0;
}





// dispatcher function, recibe las peticiones de los clientes y las envía a los servidores
int dispatcher(int argc, char *argv[])
{
	int res;
	struct ClientRequest *req;
	msg_task_t task = NULL;
	msg_task_t new_task = NULL;
	int my_d;
	char mailbox[64];
	int datacenter = 0, server = 0;
	char buf[64];

	my_d = atoi(argv[0]);
	
	sprintf(buf, "d-%d-0", my_d);
	MSG_mailbox_set_async(buf); //mailbox asincrono

	while (1)
	{
		res = MSG_task_receive_with_timeout(&(task), MSG_host_get_name(MSG_host_self()), MAX_TIMEOUT_SERVER);
		if (res != MSG_OK) break;
		req = MSG_task_get_data(task);

		// copia la tarea en otra
		new_task = MSG_task_create(MSG_task_get_name(task), MSG_task_get_flops_amount(task), 0, NULL);
		MSG_task_set_data(new_task, (void *)req);
		MSG_task_destroy(task);
		task = NULL;

		datacenter = my_d; 
		server = uniform_int(0, NUM_SERVERS_PER_DATACENTER-1);

		//printf("Tarea %s enviada al servidor s-%d-%d\n", req->id_task, datacenter, server);

		sprintf(mailbox, "s-%d-%d", datacenter, server);
		MSG_task_send(new_task, mailbox);
	}
	return 0;
}







/* server function  */
int server(int argc, char *argv[])
{
	msg_task_t task = NULL;
	msg_task_t t = NULL;
	struct ClientRequest *req;
	int res;
	int my_datacenter, my_server;
	char buf[64];

	my_datacenter = atoi(argv[0]);
	my_server = atoi(argv[1]);
	sprintf(buf, "s-%d-%d", my_datacenter, my_server);
	MSG_mailbox_set_async(buf);

	

	while (1)
	{
		res = MSG_task_receive_with_timeout(&(task), MSG_host_get_name(MSG_host_self()), MAX_TIMEOUT_SERVER);
		if (res != MSG_OK) break;
		req = MSG_task_get_data(task);

		// inserta la petición en la cola
		xbt_mutex_acquire(tasksManagement[my_datacenter].mutex[my_server]);
		tasksManagement[my_datacenter].Nqueue[my_server]++;  // un elemento mas en la cola
		tasksManagement[my_datacenter].Nsystem[my_server]++; // un elemento mas en el sistema

		xbt_dynar_push(tasksManagement[my_datacenter].client_requests[my_server], (const char *)&req);
		xbt_cond_signal(tasksManagement[my_datacenter].cond[my_server]); // despierta al proceso server
		xbt_mutex_release(tasksManagement[my_datacenter].mutex[my_server]);
		MSG_task_destroy(task);
		task = NULL;
	}

	// marca el fin
	xbt_mutex_acquire(tasksManagement[my_datacenter].mutex[my_server]);
	tasksManagement[my_datacenter].EmptyQueue[my_server] = 1;
	xbt_cond_signal(tasksManagement[my_datacenter].cond[my_server]);
	xbt_mutex_release(tasksManagement[my_datacenter].mutex[my_server]);

	return 0;
}



/* server function  */
int dispatcherServer(int argc, char *argv[])
{
	int res;
	char mailbox[64];
	struct ClientRequest *req;
	struct ServerResponse *resServer;
	msg_task_t task = NULL;
	msg_task_t ans_task = NULL;
	double Nqueue_avg = 0.0;
	double Nsystem_avg = 0.0;
	double c;
	int n_tasks = 0;
	int my_datacenter, my_server;
	
	my_datacenter = atoi(argv[0]);
	my_server = atoi(argv[1]);

	char hostS[30];
	sprintf(hostS,"s-%d-%d",my_datacenter,my_server);
	msg_host_t host = MSG_host_by_name(hostS);

	while (1)
	{

		xbt_mutex_acquire(tasksManagement[my_datacenter].mutex[my_server]);

		while ((tasksManagement[my_datacenter].Nqueue[my_server] == 0) && (tasksManagement[my_datacenter].EmptyQueue[my_server] == 0))
		{
			xbt_cond_wait(tasksManagement[my_datacenter].cond[my_server], tasksManagement[my_datacenter].mutex[my_server]);
		}

		if ((tasksManagement[my_datacenter].EmptyQueue[my_server] == 1) && (tasksManagement[my_datacenter].Nqueue[my_server] == 0))
		{
			xbt_mutex_release(tasksManagement[my_datacenter].mutex[my_server]);
			break;
		}

		// extrae un elemento de la cola
		xbt_dynar_shift(tasksManagement[my_datacenter].client_requests[my_server], (char *)&req);
		tasksManagement[my_datacenter].Nqueue[my_server]--; // un elemento menos en la cola
		n_tasks++;

		// calculo de estadisticas
		tasksManagement[my_datacenter].Navgqueue[my_server] = (tasksManagement[my_datacenter].Navgqueue[my_server] * (n_tasks - 1) + tasksManagement[my_datacenter].Nqueue[my_server]) / n_tasks;
		tasksManagement[my_datacenter].Navgsystem[my_server] = (tasksManagement[my_datacenter].Navgsystem[my_server] * (n_tasks - 1) + tasksManagement[my_datacenter].Nsystem[my_server]) / n_tasks;
		xbt_mutex_release(tasksManagement[my_datacenter].mutex[my_server]);

		// crea una tarea para su ejecución
		task = MSG_task_create("task", req->t_service, 0, NULL);
		MSG_task_execute(task);

		//printf("%s\n",MSG_host_get_name(host));

		printf("Task done in %s (duration: %.2f s). Current peak speed=%.0E flop/s; Current consumption: from %.0fW to %.0fW"
		" depending on load; Energy dissipated=%.0f J\n\n",
		MSG_host_get_name(host), MSG_get_clock() - req->t_arrival, MSG_host_get_speed(host), sg_host_get_wattmin_at(host, MSG_host_get_pstate(host)),
		sg_host_get_wattmax_at(host, MSG_host_get_pstate(host)), sg_host_get_consumed_energy(host));



		xbt_mutex_acquire(tasksManagement[my_datacenter].mutex[my_server]);
		tasksManagement[my_datacenter].Nsystem[my_server]--; // un elemento menos en el sistema
		xbt_mutex_release(tasksManagement[my_datacenter].mutex[my_server]);
		c = MSG_get_clock(); // tiempo de terminacion de la tarea
		
		resServer = (struct ServerResponse *)malloc(sizeof(struct ServerResponse));
		avServTime[my_datacenter].avServiceTime += (c - (req->t_arrival));
		avServTime[my_datacenter].numTasks += 1;
		resServer->server_cluster = my_datacenter;
		resServer->server = my_server;

		ans_task = MSG_task_create(MSG_task_get_name(task), MSG_task_get_flops_amount(task), 0, NULL);
		sprintf(resServer->response, "Task finished on %d-%d", resServer->server_cluster, resServer->server);

		sprintf(resServer->id_task,"%s",req->id_task);
		sprintf(mailbox, "c-%d-%d", req->iot_cluster, req->device);
		MSG_task_set_data(ans_task, (void *)resServer);

		/*Reenvio de terminacion de la tarea al cliente*/
		MSG_task_send(ans_task, mailbox);
		//free(req);
		MSG_task_destroy(task);
	}
}





void test_all(char *file)
{
	int argc;
	char str[50];
	int i, j;
	msg_process_t p;

	MSG_create_environment(file);

	// el proceso client es el que genera las tareas en los dispositivos IoT
	MSG_function_register("client", client);
	
	// el proceso dispatcher es el que distribuye las peticiones que le llegan a los Datacenters
	MSG_function_register("dispatcher", dispatcher);

	// cada servidor tiene un proceso server que recibe las peticiones: server
	// y un proceso dispatcher que las ejecuta
	MSG_function_register("server", server);
	MSG_function_register("dispatcherServer", dispatcherServer);

	for(i = 0; i < NUM_DATACENTERS; i++)
	{
		for (j = 0; j < NUM_SERVERS_PER_DATACENTER; j++)
		{
			sprintf(str, "s-%d-%d", i, j);
			argc = 2;
			char **argvc = xbt_new(char *, 3);

			argvc[0] = bprintf("%d", i);
			argvc[1] = bprintf("%d", j);
			argvc[2] = NULL;

			p = MSG_process_create_with_arguments(str, server, NULL, MSG_get_host_by_name(str), argc, argvc);
			
			if (p == NULL)
			{
				printf("Error en ......... s-%d-%d\n", i, j);
				exit(0);
			}
		}
	}
	

	for(i = 0; i < NUM_DATACENTERS; i++)
	{
		for (j = 0; j < NUM_SERVERS_PER_DATACENTER; j++)
		{
			sprintf(str, "s-%d-%d", i, j);
			argc = 2;
			char **argvc = xbt_new(char *, 3);

			argvc[0] = bprintf("%d", i);
			argvc[1] = bprintf("%d", j);
			argvc[2] = NULL;
			
			p = MSG_process_create_with_arguments(str, dispatcherServer, NULL, MSG_get_host_by_name(str), argc, argvc);
			if (p == NULL)
			{
				printf("Error en ......... s-%d-%d\n", j);
				exit(0);
			}
		}
	}



	for(i = 0; i < NUM_IOT_CLUSTERS; i++)
	{
		for (j = 0; j < NUM_DEVICES_PER_IOT_CLUSTER; j++)
		{
			sprintf(str, "c-%d-%d", i,j);
			argc = 2;
			char **argvc = xbt_new(char *, 3);

			argvc[0] = bprintf("%d", i);
			argvc[1] = bprintf("%d", j);
			argvc[2] = NULL;

			p = MSG_process_create_with_arguments(str, client, NULL, MSG_get_host_by_name(str), argc, argvc);
			if (p == NULL)
			{
				printf("Error en ......... c-%d-%d\n", i, j);
				exit(0);
			}
		}
	}

	

	for (i = 0; i < NUM_DISPATCHERS; i++)
	{
		sprintf(str, "d-%d-0", i);
		argc = 1;
		char **argvc = xbt_new(char *, 2);

		argvc[0] = bprintf("%d", i);
		argvc[1] = NULL;

		p = MSG_process_create_with_arguments(str, dispatcher, NULL, MSG_get_host_by_name(str), argc, argvc);
		if (p == NULL)
		{
			printf("Error en ......... d-%d-0\n", i);
			exit(0);
		}
	}

	return;
}







/** Main function */
int main(int argc, char *argv[])
{
	msg_error_t res = MSG_OK;
	int i,j;

	double t_medio_servicio = 0.0; // tiempo medio de servicio de cada tarea
	double q_medio = 0.0;		   // tamaño medio de la cola (esperando a ser servidos)
	double n_medio = 0.0;		   // número medio de tareas en el sistema (esperando y ejecutando)

	if (argc < 4)
	{
		printf("Usage: %s platform_file lambda percentageTasks \n", argv[0]);
		exit(1);
	}

	if (atof(argv[3]) > 1 || atof(argv[3]) < 0)
	{
		printf("Percentage Tasks must be a value between 0 and 1.\n");
		exit(1);
	}

	percentageTasks = atof(argv[3]);


	seed((int)time(NULL));
	ARRIVAL_RATE = atof(argv[2]) * NUM_SERVERS_PER_DATACENTER;
	sg_host_energy_plugin_init();
	MSG_init(&argc, argv);

	for (i = 0; i < NUM_DATACENTERS; i++)
	{
		for (j = 0; j < NUM_SERVERS_PER_DATACENTER; j++)
		{
			tasksManagement[i].Nqueue[j] = 0;
			tasksManagement[i].Nsystem[j] = 0;
			tasksManagement[i].EmptyQueue[j] = 0;
			tasksManagement[i].mutex[j] = xbt_mutex_init();
			tasksManagement[i].cond[j] = xbt_cond_init();
			tasksManagement[i].client_requests[j] = xbt_dynar_new(sizeof(struct ClientRequest *), NULL);
		}
	}

	test_all(argv[1]);

	res = MSG_main();
	int totaltasks = 0;
	for (i = 0; i < NUM_DATACENTERS; i++)
	{
		for (j = 0; j < NUM_SERVERS_PER_DATACENTER; j++)
		{
			q_medio = q_medio + tasksManagement[i].Navgqueue[j];
			n_medio = n_medio + tasksManagement[i].Navgsystem[j];
		}

		t_medio_servicio = avServTime[i].avServiceTime / (avServTime[i].numTasks);
		q_medio = q_medio / (NUM_SERVERS_PER_DATACENTER);
		n_medio = n_medio / (NUM_SERVERS_PER_DATACENTER);

		printf("DATACENTER \t tiempoMedioServicio \t TamañoMediocola \t    TareasMediasEnElSistema  \t   tareas\n");
		printf("%i \t\t %g \t\t\t %g \t\t\t  %g  \t\t\t  %d \n\n", i, t_medio_servicio, q_medio, n_medio, avServTime[i].numTasks);

		t_medio_servicio = 0;
		q_medio = 0;
		n_medio = 0;
		totaltasks += avServTime[i].numTasks;
	}
	
	printf("Simulation time %g\n",MSG_get_clock());

	for(i = 0; i < NUM_DATACENTERS; i++)
	{
		for (j = 0; j < NUM_SERVERS_PER_DATACENTER; j++)
		{
			xbt_dynar_free(&tasksManagement[i].client_requests[j]);
		}
	}
	

	if (res == MSG_OK)
		return 0;
	else
		return 1;
}