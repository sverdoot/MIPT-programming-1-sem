#include "Dotter.h"

int dump();

int dump ()
{
    dtBegin ("image of tree.dot"); //������ dot �����

    dtNodeStyle ().shape ("box"); //������ ����� �� box

    dtNode (0, "i = 0\n value = 1"); //������ ���� 0
    dtNode (1, "i = 1\n value = 3"); //������ ���� 1
    dtNode (2, "i = 2\n value = 9"); //������ ���� 2
    dtNode (3, "i = 3\n value = 5"); //������ ���� 3
    dtNode (4, "i = 4\n value = 6"); //������ ���� 4
    dtNode (5, "i = 5\n value = 4"); //������ ���� 5
    dtNode (6, "i = 6\n value = 4"); //������ ���� 6

    dtLinkStyle ().style ("dashed"); //������ ����� �� dashed

    dtLink (0, 1); //������ ����� ���� 0 � 1
    dtLink (0, 2); //������ ����� ���� 0 � 2
    dtLink (0, 3); //������ ����� ���� 0 � 3

    dtLink (3, 4); //������ ����� ���� 3 � 4
    dtLink (3, 5); //������ ����� ���� 3 � 5

    dtLink (5, 6); //������ ����� ���� 5 � 6

    dtLinkStyle ().style ("bold") //������ ����� �� bold
                  .color ("red"); //������ ���� �� red

    dtLink (4, 6, "Error"); //������ ����� ���� 4 � 6

    dtEnd (); //����� dot �����

    dtRender ("image of tree.dot"); //������ �� ����� ��������
}
