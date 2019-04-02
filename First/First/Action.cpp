#include "Action.h"
#include "Warrior.h"

Action::Action(Warrior& warrior, eType type): warrior(warrior), type(type)
{
	updateScore();
}

void Action::updateScore() 
{
	double val = calculateScore(type, warrior);
	if (val >= 0 && val <= 100)
		this->score = val;
	/*else
		throw exception("The function Action::updateScore get invalide value");*/
}

Warrior& Action::getWarrior() const
{
	return warrior;
}


Action::eType Action::getType() const
{
	return type;
}


double Action::calculateScore(Action::eType type, Warrior & warrior)
{
	//eType::RUN;
	switch (type)
	{
	case Action::RUN:
		return calculateScoreRun(warrior);
		break;
	case Action::FIND_AMMO:
		return warrior.getAmmoP() * calculateScoreFindAmmo(warrior);
		break;
	case Action::FIND_MED:
		return warrior.getMedP() * calculateScoreFindMed(warrior);
		break;
	case Action::FIGHT:
		return warrior.getFightP() * calculateScoreFight(warrior);
		break;
	default:
		break;
	}
	return 0;
}



/*f(w) = (maxGuns - currentGuns + maxGrandes - currentGrandes)/(MaxGuns + MaxGreandes) * 100 */
double Action::calculateScoreFindAmmo(Warrior & w)
{
	double maxGuns = w.getMaxGuns(), currentGuns = w.getGunsAmmo(),
		maxGrandes = w.getMaxGrandes(), currentGrandes = w.getGrenadeAmmo();
	return ((maxGuns - currentGuns) + (maxGrandes - currentGrandes)) / (maxGuns + maxGrandes) * 100;
}

/*f(w) = (MaxLife - currentLife)/MaxLife * 100 */
double Action::calculateScoreFindMed(Warrior & w)
{
	double maxLife = w.getMaxLife(), life = w.getlifePoints();
	return (maxLife - life) / maxLife * 100;
}


/*f(w) = (calculateMedical + calculateAmmo + seftyScore) /(3 *maxScoreType) * 100 */
double Action::calculateScoreRun(Warrior & w)
{
	double medScore = calculateScoreFindMed(w), ammoScore = calculateScoreFindAmmo(w), 
		seftyScore = (&Maze::getInstance())->getSaftyScore(w.getLocation());
	return (medScore + ammoScore + seftyScore ) /
		(MAX_SCORE + MAX_SCORE + MAX_SAFTY_SCORE) * 100;
}
/*f(w) = MAX_SCORE - calcualteScoreRun */

double Action::calculateScoreFight(Warrior & w)
{
	return MAX_SCORE - calculateScoreRun(w) ;
}

/*???*/
double Action::calculateScoreHurt(Warrior & w)
{
	return 0.0;
}
