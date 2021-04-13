#include "Road.h"

Road::Road()
{
	int roadType = 0, fragmentNum = 0, numLines = 0;
	FILE * file = fopen("Source\\Laboratoare\\Laborator6\\input.txt", "r");
	float fragsSize = 0;
	if (file != NULL)
	{
		fscanf(file, "%d", &numLines);
		while (numLines > 0)
		{
			fscanf(file, "%d", &roadType);
			fscanf(file, "%d", &fragmentNum);
			numLines -= 1;
			
			switch (roadType) {
			case 1:
				if(this->frags.size() == 0)
					this->straightRoad(fragmentNum, glm::vec3(0.0f, 0.0f, 0.5f));
				else
				{
					fragsSize = this->frags.size() - 1;
					this->straightRoad(fragmentNum, glm::vec3((this->frags[fragsSize].centre.x), 0.0f, this->frags[fragsSize].centre.z));
				}
				break;
			case 2:
				if (this->frags.size() == 0)
					this->leftCurve(fragmentNum, glm::vec3(0.0f, 0.0f, 0.5f));
				else
				{
					fragsSize = this->frags.size() - 1;
					this->leftCurve(fragmentNum, glm::vec3((this->frags[fragsSize].centre.x), 0.0f, this->frags[fragsSize].centre.z));
				}
				break;
			case 3:
				if (this->frags.size() == 0)
					this->rightCurve(fragmentNum, glm::vec3(0.0f, 0.0f, 0.5f));
				else
				{
					fragsSize = this->frags.size() - 1;
					this->rightCurve(fragmentNum, glm::vec3((this->frags[fragsSize].centre.x), 0.0f, this->frags[fragsSize].centre.z));
				}
				break;
			}

		}

		fclose(file);
	}

}

void Road::straightRoad(int nrFragments, glm::vec3 centre)
{	
	int color = 1;
	glm::vec3 auxCentre = glm::vec3(centre.x, centre.y, centre.z - 1.0f);
	Fragment f = Fragment(color);
	for (int i = 0;i < nrFragments; i++ )
	{
		f = Fragment(color);
		f.translate(glm::vec3(auxCentre.x, auxCentre.y, auxCentre.z));
		frags.push_back(f);
		auxCentre = glm::vec3(auxCentre.x, auxCentre.y, auxCentre.z - 1.0f);
		if (color == 1)
			color = 0;
		else
			color = 1;
	}
}

void Road::leftCurve(int nrFragments, glm::vec3 centre)
{
	int color = 1;
	glm::vec3 auxCentre = glm::vec3(centre.x - 0.5f, centre.y, centre.z - 1.0f);
	Fragment f = Fragment(color);
	for (int i = 0; i < nrFragments; i++)
	{
		f = Fragment(color);
		f.translate(glm::vec3(auxCentre.x, auxCentre.y, auxCentre.z));
		frags.push_back(f);
		auxCentre = glm::vec3(auxCentre.x - 0.5f, auxCentre.y, auxCentre.z - 1.0f);
		if (color == 1)
			color = 0;
		else
			color = 1;
	}
}

void Road::rightCurve(int nrFragments, glm::vec3 centre)
{
	int color = 1;
	glm::vec3 auxCentre = glm::vec3(centre.x + 0.5f, centre.y, centre.z - 1.0f);
	Fragment f = Fragment(color);
	for (int i = 0; i < nrFragments; i++)
	{
		f = Fragment(color);
		f.translate(glm::vec3(auxCentre.x, auxCentre.y, auxCentre.z));
		frags.push_back(f);
		auxCentre = glm::vec3(auxCentre.x + 0.5f, auxCentre.y, auxCentre.z - 1.0f);
		if (color == 1)
			color = 0;
		else
			color = 1;
	}
}