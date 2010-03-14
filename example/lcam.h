#ifndef __lcam_h
#define __lcam_h
//**************************************************************//
//* Header de la librairie c pour la TSL3301                    //
//* Auteur(s): Eric Seuret                                      //
//* Date: 04-05-2007                                            //
//* Version: 0.1                                                //
//**************************************************************//

/** \defgroup lcam_h Cam�ra lin�aire
	
	\brief	Librairie pour l'utilisation de la cam�ra lin�aire

	\author Christophe Winter (code)
	\author	Fabrizio Loconte (code)
	\author	Eric Seuret (code+documentation)
	\date	Octobre 2007

*/
/*@{*/


/** \brief Configuration des ports du microcontr�leur
	
	Cette fonction configure les pins sur lesquels la cam�ra est branch�e en entr�e ou en sortie. 
	Vous devez l'appeler apr�s avoir fait toute vos modification sur les ports, elle ne modifiera que ce qu'elle doit modifier.


*/
void lcam_initport(void);



/** \brief Initialisation de la cam�ra avec des param�tres standards

	Cette fonction initialise les parama�tre de la cam�ra (offset et gain) avec des param�tres standard.

*/
void lcam_setup(void);




/** \brief Reset de la camera

	Envoi l'ordre � la cam�ra de faire un reset

*/
void lcam_reset(void);


/** \brief D�but de l'acquisition d'une image

	Cette fonction envoie l'ordre � la cam�ra de commencer l'acquisition d'une image. Lors de la r�ception de cette ordre la cam�ra commence la prise d'une image.

*/
void lcam_startintegration(void);


/** \brief Fin de l'int�gration et t�l�chargement des donn�es

	Cette fonction donne l'ordre � la cam�ra de stopper l'acquision d'une image et d'envoyer l'image au microcontr�leur. Elle se charge ensuite du t�l�chargement
	des donn�e

	\param image Pointeur vers la zone m�moire ou l'image sera enregistr�e (doit contenir 102 pixels)

*/
void lcam_stop(unsigned char *image);

void lcam_endintegration(void);	//Fin de l'int�gration
void lcam_readout(void);		//Pr�paration � la lecture
void lcam_read(unsigned char *image); //Lecture et sauvegarde dans buffer


/** \brief Fonction pour la recherche d'un pic

	Cette fonction recherche le pic de plus haute valeur sur une image. Cette fonction divise les 102 pixels en 25 zones num�rot�e de 1 � 25. Le r�sultat obtenu
	est le num�ro de la zone la plus lumineuse ou 0 si aucune zone n'�tait significativement plus lumineuse que les autres.

	Il faut �galement noter que cette fonction modifie l'image en m�moire. Une fois appliqu� l'image n'est plus la m�me.

	\param image Pointeur vers la zone m�moire contenant l'image � analyser (doit contenir 102 pixels)
	\return 0 Aucune zone lumineuse d�tecter
	\return # num�ro de la zone la plus lumineuse


*/
unsigned char lcam_getpic(unsigned char *image);
/*@}*/

#endif

/** \addtogroup lcam_h
	\section lcamexemple Exemple de programme document�

	Il ne faut pas oublier d'ajouter les fichier lcam.s et lcamc.c au projet sous AVR Studio.

	\code

	#include <avr/io.h>
	#include <util/delay.h> //pour les fonctions _delay_ms() et _delay_us()
	#include "lcam.h"

	int main(){
		unsigned char image[102];	//Tableau de 102 valeurs pour pouvoir contenir les 102 pixels
		unsigned char valeur;		//Valeur de retour de la fonction lcam_getpic

		//Initialisation
		lcam_initport();		//Initialisation des pins du port
		lcam_reset();			//Ordonne un reset � la camera
		lcam_setup();			//Configure la cam�ra
		
		_delay_ms(2);			//attendre 2 ms pour que la logique de la camera puisse se configurer
			

		while(1){
			
			lcam_startintegration();	//On commence l'acquisition
			
			_delay_us(96);		//Ici il faut attendre le temps d'int�gration, c-�-d le temps d'exposition.
			_delay_us(96);		//400 us est en g�n�ral une bonnes valeur
			_delay_us(96);		//Mais comme en photographie cela d�pends de divers param�tres dont votre optique
			_delay_us(96);		//et nous vous conseillons de faire quelques tests
			_delay_us(15);		//_delay_us() peut faire au maximum 96us sur un microcontr�leur � 8 Mhz c'est pourquoi nous l'avons r�p�ter
			
			lcam_stop(image);  		//Fin de l'acquisition et t�l�chargement de l'image
			valeur = lcam_getpic(image); 	//Traitement de l'image 
			

			//////////////////////////////////////////
			//INSEREZ ICI CE QUE VOUS VOULEZ FAIRE
			//AVEC LE RESULTAT
			/////////////////////////////////////////
			
			_delay_ms(30);		//Attendre 100 ms avant la prochaine acquisiton
			_delay_ms(30);		//La cam�ra ne peut fournir qu'une dizaine d'image par secondes (sinon elle plante)
			_delay_ms(30);		//Il y a plusieur _delay_ms() car cette fonciton peut attendre au maximum 30ms 
			_delay_ms(10);		//sur un microcontr�leur � 8 Mhz
		}


		
		return 0;
	}



	\endcode

	*/


