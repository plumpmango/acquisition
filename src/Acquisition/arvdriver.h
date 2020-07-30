/*!
 * \file arvdriver.h
 * \brief fichier de la librairie aravis-0.6
 * \author modifié par Chafik
 * \version 0
 */

#include <arv.h>

//opencv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <queue>
#include <iostream>


using namespace cv;

/*! \struct ApplicationData
* \brief Objet relatif aux données d'une vidéo
*
*/
typedef struct {
    GMainLoop *main_loop; /*!<type de données opaques représentant la boucle principale d'une application GLib ou GTK*/
    int buffer_count;/*!<compteur de buffer*/
    int frame_number;/*!<nombre d'image*/
    ArvBuffer *last_buffer;/*!< ajouté par chafik*/
	  ArvChunkParser *chunk_parser;/*!<analyseur de token*/
	  char **chunks;/*!<tokens*/
} ApplicationData;

/*! \class ArvDriver
* \brief classe qui permet de faire de l'acquisition de vidéo en direct
* depuis une caméra ou une wwebcam
*/

class ArvDriver
{

public:
	/*! \brief Constructeur
	* Initialise une instance de ApplicationData
	*/
	ArvDriver(char *cameraName,double frequency = 5,gint64 delay = 25);
	/*! \brief Destructeur
	*/
    ~ArvDriver();
	/*! \brief Lance l'acquisition des images sur la première caméra disponible
    */
	void startAcquisition();
    /*! \brief Arrête l'acquisition des images depuis la caméra
	*/
	void stopAcquisition();
    /*! \brief Recupère l'image courante de la vidéo depuis le buffer qui les stocke
	* \return image courante nommé par son numéro dans la vidéo
	*/
	Mat getImageFromBuffer();
	void set_cancel (int signal);
	void control_lost_cb (ArvGvDevice *gv_device);

    //static methods
    static gboolean emit_software_trigger (void *abstract_data);
    static void new_buffer_cb (ArvStream *stream, ApplicationData *data);
	static void stream_cb (void *user_data, ArvStreamCallbackType type, ArvBuffer *buffer);

private:
	char *arv_option_camera_name;
	char *arv_option_debug_domains = NULL;
	gboolean arv_option_snaphot = FALSE;
	char *arv_option_trigger = NULL;
	double arv_option_software_trigger = -1;
	double arv_option_frequency;
	int arv_option_width = -1;
	int arv_option_height = -1;
	int arv_option_horizontal_binning = 0;
	int arv_option_vertical_binning = 0;
	double arv_option_exposure_time_us = -1;
	int arv_option_gain = -1;
	gboolean arv_option_auto_socket_buffer = FALSE;
	gboolean arv_option_no_packet_resend = FALSE;
	double arv_option_packet_request_ratio = -1.0;
	unsigned int arv_option_packet_timeout = 20;
	unsigned int arv_option_frame_retention = 100;
	int arv_option_gv_stream_channel = -1;
	gint64 arv_option_gv_packet_delay; //modified by chafik (if value is diffrent from 0 it produces jerks)
  int arv_option_gv_packet_size = -1;
	gboolean arv_option_no_packet_socket = FALSE;
	char *arv_option_chunks = NULL;
	unsigned int arv_option_bandwidth_limit = -1;
  guint software_trigger_source=0;
	ArvCamera *camera=NULL;
	ArvStream *stream=NULL;

    //initialized in the consturctor
    ApplicationData data;

    //static attributes
	static gboolean arv_option_realtime;
	static gboolean arv_option_high_priority;


};
