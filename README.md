# MLV_challenge


Le corona challenge, pour calmer les gens énervés durant le confinement...


## Qu'est ce que c'est que cette ânerie ?


C'est une vieille arène de robots dans l'espace codée en libMLV. Ça a
failli devenir un projet L3 mais ça aurait été un trop petit projet
sans devoir coder l'arène. En rajoutant l'arène dans le sujet, les
élèves n'aurait pas produit des A.I. interopérables toutes ensembles
car spécifier complètement une arène est chose... difficile.


Comme je sens que le confinement vous énerve, venez donc coder une
A.I. pour prouver que vous n'êtes pas des informaticiens de niveau 1.


Vous avez besoin de la libMLV pour installer l'arène et les A.I. de
démonstration. Il suffit de faire make pour construire tout le
projet. Une vieille documentation horrible (qui n'a jamais été
terminée est disponible ici (cette vieille doc n'est rien d'autre
qu'un début de doxygen inclus dans le dépôt):
https://nborie.github.io/MLV_challenge/doc/html/


## Quelques règles... 


les règles sont susceptibles d'évoluer légèrement seulement pour le
bien du challenge...


* 100 Méga-octets max de mémoire par A.I. (c'est déjà énorme!!!).
* Les A.I. seront disqualifiées si elle ralentissent le programme.
* On ne triche pas en essayant de passer au dessus de l'A.P.I. (seules
  les fonctions publiées du module acquisition.h sont autorisées).
* On gagne quoi qu'il arrive le droit de rejouer (quelle classe quand
  même...).
* Je vais bien finir par trouver des lots (tasses UPEM, clé usb, vieux
  goodies, ...)
* Votre A.I. doit être la plus performante possible en 1 vs 1, 2 vs 2
  et 3 vs 3 (peut-être qu'il ne faut pas tirer n'importe comment...).
* Il y a du friendly fire. 
* Tout le code dégueulasse (de 2014 je crois...) de l'arène est ici
  publiquement sur github.
* Il n'y a que trois fonctions à écrire par A.I., votre vaisseau doit
  prendre une décision à chaque frame.
* Si vous avez des propositions d'amélioration intéressante qui ne
  casse pas la challenge, je regarderai soigneusement les pull requests !
* Les exemples d'A.I. triviales et moches peuvent vous aider pour
  savoir comment interagir poliment avec l'arène.
* La vision est en cône infini, selon un radar orientable.


## Le timing


Suivant l'énervement des gens et la durée du confinement, une limite
raisonnable sera établie. Attention, on n'a le droit de jouer que
lorsque l'on a fini son travail de classe standard de manière
générale.


## Les changements de code mineur de l'arène 

La rétro-compatibilité ne sera jamais cassé pour les plugins (les
AI). En gros, le module acquisition.h ne bougera pas. L'arène pourra
évoluer juste pour devenir plus agréable.


Amélioration à venir :


* colorisation par équipe : https://github.com/nborie/MLV_challenge/issues/1



N. Borie
