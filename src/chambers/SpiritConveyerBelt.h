/*
	SPIRIT CONVEYER BELT

	One conveyer belt implemented using a queue.
	
	Easy Mode:
	There are 4 spirits: Fire, Water, Grass, Earth.

	Fire cannot be next to water.
	Grass cannot be next to fire.
	Water cannot be next to earth.
	Grass wants to be next to water.

	Correct Solutions:
	Water Grass Earth Fire
	Fire Earth Grass Water

	
	Hard Mode:

	7 Spirits: Fire, Water, Grass, Earth, Ice, Light, Dark

	
	Fire cannot be next to water.
	Grass cannot be next to fire.
	Ice cannot be next to fire.
	Light cannot be next to dark.
	Grass cannot be next to dark.
	Fire cannot be next to Dark.

	Earth cannot be next to Ice.
	Earth cannot be next to water.

	Grass wants to be next to water.
	Grass wants to be next to light.
	Grass cannot be next to ice.
	
*/

class Spirit
{
public:
	string name;
	bool used;
	Spirit(string name) : name(name), used(false) {}
};

enum Difficulty
{
	EASY,
	HARD
};

class SpiritConveyerBelt 
{
	Queue <Spirit> conveyerBelt;
	Spirit** ArrayofSolutions;
	Spirit* UserAnswer;
	bool SolutionFound;

	SpiritConveyerBelt::~SpiritConveyerBelt()
	SpiritConveyerBelt(Difficulty diff);
	bool CheckAnswer();
	bool match(Spirit User, Spirit System);
	void insert(Spirit key);
	void undo();
};