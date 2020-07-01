package spawn_scenario;

import moving_objects.EnemyShip;

import java.util.LinkedList;

public interface Scenario {
    public boolean spawn(LinkedList<EnemyShip> holdEnemies, LinkedList<EnemyShip> flyingEnemies, int fieldWidth);
}
