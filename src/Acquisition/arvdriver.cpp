#include "arvdriver.h"


gboolean ArvDriver::arv_option_realtime = FALSE;
gboolean ArvDriver::arv_option_high_priority = FALSE;


void ArvDriver::new_buffer_cb (ArvStream *stream, ApplicationData *data)
{
	ArvBuffer *buffer;

	buffer = arv_stream_try_pop_buffer (stream);
	if (buffer != NULL) {

		if (arv_buffer_get_payload_type (buffer) == ARV_BUFFER_PAYLOAD_TYPE_CHUNK_DATA &&
		    data->chunks != NULL) {
			int i;
			for (i = 0; data->chunks[i] != NULL; i++)
				printf ("%s = %" G_GINT64_FORMAT "\n", data->chunks[i],
					arv_chunk_parser_get_integer_value (data->chunk_parser, buffer, data->chunks[i]));
		}

		if (data->last_buffer != NULL)
			arv_stream_push_buffer (stream, data->last_buffer);
		data->last_buffer = buffer;

		if (arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_SUCCESS)
			data->buffer_count++;

	}
}

void ArvDriver::stream_cb (void *user_data, ArvStreamCallbackType type, ArvBuffer *buffer)
{
	if (type == ARV_STREAM_CALLBACK_TYPE_INIT) {
		if (ArvDriver::arv_option_realtime) {
			if (!arv_make_thread_realtime (10))
				printf ("Failed to make stream thread realtime\n");
		} else if (ArvDriver::arv_option_high_priority) {
			if (!arv_make_thread_high_priority (-10))
				printf ("Failed to make stream thread high priority\n");
		}
	}
}


gboolean ArvDriver::emit_software_trigger (void *abstract_data)
{
	ArvCamera *camera = (ArvCamera *)abstract_data;  //modified by chafik

	arv_camera_software_trigger (camera);

	return TRUE;
}



ArvDriver::ArvDriver(char *cameraName)
{
	arv_option_camera_name = cameraName;
	data.buffer_count = 0;
  data.frame_number = 0;
  data.last_buffer=NULL;
	data.chunks = NULL;
	data.chunk_parser = NULL;

}

ArvDriver::~ArvDriver()
{

}

void ArvDriver::startAcquisition()
{

	GError *error = NULL;
	int i;

	data.buffer_count = 0;

    data.frame_number = 0;   //added by chafik

	data.chunks = NULL;
	data.chunk_parser = NULL;

	arv_g_thread_init (NULL);
	arv_g_type_init ();

	arv_debug_enable (arv_option_debug_domains);
	printf("'%s'\n", arv_option_camera_name);

	if (arv_option_camera_name == NULL)
		g_print ("Looking for the first available camera\n");
	else
		g_print ("Looking for camera '%s'\n", arv_option_camera_name);

	camera = arv_camera_new (arv_option_camera_name);
	g_print("camera '%s'\n", arv_option_camera_name);
	if (camera != NULL) {
		gint payload;
		gint x, y, width, height;
		gint dx, dy;
		double exposure;
		guint64 n_completed_buffers;
		guint64 n_failures;
		guint64 n_underruns;
		int gain;
		software_trigger_source = 0;

		if (arv_option_chunks != NULL) {
			char *striped_chunks;

			striped_chunks = g_strdup (arv_option_chunks);
			arv_str_strip (striped_chunks, " ,:;", ',');
			data.chunks = g_strsplit_set (striped_chunks, ",", -1);
			g_free (striped_chunks);

			data.chunk_parser = arv_camera_create_chunk_parser (camera);

			for (i = 0; data.chunks[i] != NULL; i++) {
				char *chunk = g_strdup_printf ("Chunk%s", data.chunks[i]);

				g_free (data.chunks[i]);
				data.chunks[i] = chunk;
			}
		}

		arv_camera_set_chunks (camera, arv_option_chunks);
		arv_camera_set_region (camera, 0, 0, 2064, 1544);  //modified by chafik
		arv_camera_set_binning (camera, arv_option_horizontal_binning, arv_option_vertical_binning);
		arv_camera_set_exposure_time (camera, 7500);  // 1500
		arv_camera_set_gain (camera, arv_option_gain);
                arv_camera_set_pixel_format(camera, ARV_PIXEL_FORMAT_BAYER_BG_8); //added by chafik
                arv_camera_set_frame_rate (camera, 30); //added by chafik
                arv_camera_gv_auto_packet_size(camera); //added by chafik


		if (arv_camera_is_uv_device(camera)) {
			arv_camera_uv_set_bandwidth (camera, arv_option_bandwidth_limit);
		}
		
		if (arv_camera_is_gv_device (camera)) {
			arv_camera_gv_select_stream_channel (camera, arv_option_gv_stream_channel);
			arv_camera_gv_set_packet_delay (camera, arv_option_gv_packet_delay);
			arv_camera_gv_set_packet_size (camera, arv_option_gv_packet_size);
			arv_camera_gv_set_stream_options (camera, arv_option_no_packet_socket ?
							  ARV_GV_STREAM_OPTION_PACKET_SOCKET_DISABLED :
							  ARV_GV_STREAM_OPTION_NONE);
		}

		arv_camera_get_region (camera, &x, &y, &width, &height);

		arv_camera_get_binning (camera, &dx, &dy);
		exposure = arv_camera_get_exposure_time (camera);
		payload = arv_camera_get_payload (camera);
		gain = arv_camera_get_gain (camera);

		printf ("vendor name           = %s\n", arv_camera_get_vendor_name (camera));
		printf ("model name            = %s\n", arv_camera_get_model_name (camera));
		printf ("device id             = %s\n", arv_camera_get_device_id (camera));
		printf ("image width           = %d\n", width);
		printf ("image height          = %d\n", height);
		printf ("horizontal binning    = %d\n", dx);
		printf ("vertical binning      = %d\n", dy);
		printf ("payload               = %d bytes\n", payload);
		printf ("exposure              = %g µs\n", exposure);
		printf ("gain                  = %d dB\n", gain);

		if (arv_camera_is_gv_device (camera)) {
			printf ("gv n_stream channels  = %d\n", arv_camera_gv_get_n_stream_channels (camera));
			printf ("gv current channel    = %d\n", arv_camera_gv_get_current_stream_channel (camera));
			printf ("gv packet delay       = %" G_GINT64_FORMAT " ns\n", arv_camera_gv_get_packet_delay (camera));
			printf ("gv packet size        = %d bytes\n", arv_camera_gv_get_packet_size (camera));
		}

		if (arv_camera_is_uv_device (camera)) {
			guint min,max;

			arv_camera_uv_get_bandwidth_bounds (camera, &min, &max);
			printf ("uv bandwidth limit     = %d [%d..%d]\n", arv_camera_uv_get_bandwidth (camera), min, max);
		}

		stream = arv_camera_create_stream (camera, stream_cb, NULL);
		if (stream != NULL) {
			if (ARV_IS_GV_STREAM (stream)) {
				if (arv_option_auto_socket_buffer)
					g_object_set (stream,
						      "socket-buffer", ARV_GV_STREAM_SOCKET_BUFFER_AUTO,
						      "socket-buffer-size", 0,
						      NULL);
				if (arv_option_no_packet_resend)
					g_object_set (stream,
						      "packet-resend", ARV_GV_STREAM_PACKET_RESEND_NEVER,
						      NULL);
				if (arv_option_packet_request_ratio >= 0.0)
					g_object_set (stream,
						      "packet-request-ratio", arv_option_packet_request_ratio,
						      NULL);

				g_object_set (stream,
					      "packet-timeout", (unsigned) arv_option_packet_timeout * 1000,
					      "frame-retention", (unsigned) arv_option_frame_retention * 1000,
					      NULL);
			}

			for (i = 0; i < 50; i++)
				arv_stream_push_buffer (stream, arv_buffer_new (payload, NULL));

			arv_camera_set_acquisition_mode (camera, ARV_ACQUISITION_MODE_MULTI_FRAME);

			if (arv_option_frequency > 0.0)
				arv_camera_set_frame_rate (camera, arv_option_frequency);

			if (arv_option_trigger != NULL)
				arv_camera_set_trigger (camera, arv_option_trigger);

			if (arv_option_software_trigger > 0.0) {
				arv_camera_set_trigger (camera, "Software");
				software_trigger_source = g_timeout_add ((double) (0.5 + 1000.0 /
										   arv_option_software_trigger),
									 emit_software_trigger, camera);
			}

			arv_camera_start_acquisition (camera);

			g_signal_connect (stream, "new-buffer", G_CALLBACK (new_buffer_cb), &data);
			arv_stream_set_emit_signals (stream, TRUE);

			data.main_loop = g_main_loop_new (NULL, FALSE);

                        //wait until stop acquisition
			g_main_loop_run (data.main_loop);

                        //when stop acquisition
			if (software_trigger_source > 0)
				g_source_remove (software_trigger_source);

			g_main_loop_unref (data.main_loop);

			arv_camera_stop_acquisition (camera);

			arv_stream_set_emit_signals (stream, FALSE);

			g_object_unref (stream);


		} else
			printf ("Can't create stream thread (check if the device is not already used)\n");

		g_object_unref (camera);
	} else
		printf ("No camera found\n");


	if (data.chunks != NULL)
		g_strfreev (data.chunks);

	g_clear_object (&data.chunk_parser);

        g_clear_object (&data.last_buffer);

        printf("acquisition stopped\n");
}

void ArvDriver::stopAcquisition()
{
	g_main_loop_quit (data.main_loop);
}

// added by chafik
Mat ArvDriver::getImageFromBuffer()
{

        Mat frame;

        if ( data.buffer_count > 0 )
        {
                if(data.last_buffer!=NULL)
                {

			int width, height;
			size_t size;
			const char *image_data;

			data.frame_number++;

			printf("get frame %d from buffer\n", data.frame_number);

			arv_buffer_get_image_region (data.last_buffer, NULL, NULL, &width, &height);

			image_data = (const char*)arv_buffer_get_data ((ArvBuffer *)data.last_buffer, &size);
			frame = cv::Mat(height,width,CV_8UC1,(void*)image_data);
			cvtColor(frame,frame,COLOR_BayerBG2BGR);
			//resize(frame,frame,Size(416,416));

		}
        }

	return frame;

}
