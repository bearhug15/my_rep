package spawn_scenario;

import moving_objects.EnemyShip;

import java.util.LinkedList;

public class SimpleSpawnScenario implements Scenario {
    @Override
    public boolean spawn(LinkedList<EnemyShip> holdEnemies, LinkedList<EnemyShip> flyingEnemies, int fieldWidth) {
        if(holdEnemies.isEmpty()){
            return false;
        }
        EnemyShip spawnShip = holdEnemies.removeFirst();
        int R = spawnShip.getR();
        if(fieldWidth -2*R<0){
            throw new RuntimeException("Your screen is too small");
        }
        int spawnX = (int)(Math.random()*(fieldWidth - 2*R)) +R;
        spawnShip.setNewCoords(spawnX,R+5);
        spawnShip.changeSpeed(0,spawnShip.getDefaultSpeedY());
        spawnShip.setHealth(spawnShip.getHoldHealth());
        flyingEnemies.add(spawnShip);
        return true;
    }
}
