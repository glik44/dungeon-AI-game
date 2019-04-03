#include "Action.h"
#include "Warrior.h"

Action::Action(Warrior& warrior, eType type): warrior(warrior), type(type)
{
	updateScore();
}

void Action::updateScore() 
{
	double val = points_Calculation(type, warrior);
	if (val >= 0 && val <= 100)
		this->score = val;
}

Warrior& Action::getWarrior() const
{
	return warrior;
}


Action::eType Action::getType() const
{
	return type;
}


double Action::points_Calculation(Action::eType type, Warrior & warrior)
{
	//eType::RUN;
	switch (type)
	{
	case Action::RUN:
		return calculate_points_Run_Mission(warrior);
		break;
	case Action::FIND_AMMO:
		return warrior.getAmmoP() * calculate_points_Find_Ammo_Mission(warrior);
		break;
	case Action::FIND_MED:
		return warrior.getMedP() * calculate_points_Find_Med_Mission(warrior);
		break;
	case Action::FIGHT:
		return warrior.getFightP() * calculate_Points_Fight_Mission(warrior);
		break;
	default:
		break;
	}
	return 0;
}



/*f(w) = (maxGuns - currentGuns + maxGrandes - currentGrandes)/(MaxGuns + MaxGreandes) * 100 */
double Action::calculate_points_Find_Ammo_Mission(Warrior & w)
{
	double maxGuns = w.getMaxGuns(), currentGuns = w.getGunsAmmo(),
		maxGrandes = w.getMaxGrandes(), currentGrandes = w.getGrenadeAmmo();
	return ((maxGuns - currentGuns) + (maxGrandes - currentGrandes)) / (maxGuns + maxGrandes) * 100;
}

/*f(w) = (MaxLife - currentLife)/MaxLife * 100 */
double Action::calculate_points_Find_Med_Mission(Warrior & w)
{
	double maxLife = w.getMaxLife(), life = w.getlifePoints();
	return (maxLife - life) / maxLife * 100;
}


/*f(w) = (calculateMedical + calculateAmmo + seftyScore) /(3 *maxScoreType) * 100 */
double Action::calculate_points_Run_Mission(Warrior & w)
{
	double medScore = calculate_points_Find_Med_Mission(w), ammoScore = calculate_points_Find_Ammo_Mission(w), 
		seftyScore = (&Maze::getInstance())->getSaftyScore(w.getLocation());
	return (medScore + ammoScore + seftyScore ) /
		(MAX_SCORE + MAX_SCORE + MAX_SAFTY_SCORE) * 100;
}
/*f(w) = MAX_SCORE - calcualteScoreRun */

double Action::calculate_Points_Fight_Mission(Warrior & w)
{
	return MAX_SCORE - calculate_points_Run_Mission(w) ;
}


double Action::calculate_Points_Hurt_Mission(Warrior & w)
{
	return 0.0;
}
