package spawn_scenario;

import moving_objects.EnemyShip;

import java.util.LinkedList;

public class DoubleSpawnScenario implements Scenario {

    @Override
    public boolean spawn(LinkedList<EnemyShip> holdEnemies, LinkedList<EnemyShip> flyingEnemies, int fieldWidth) {
        if(holdEnemies.size()<2){
            return false;
        }
        EnemyShip spawnShip1 = holdEnemies.removeFirst();
        EnemyShip spawnShip2 = holdEnemies.removeFirst();
        int R = spawnShip1.getR();
        if(fieldWidth -4*R<0){
            throw new RuntimeException("Your screen is too small");
        }
        int spawnX = (int)(Math.random()*(fieldWidth - 2*R)) +R;
        if(spawnX<fieldWidth/2+R && spawnX>fieldWidth/2-R){
            spawnX-=R/2;
        }
        spawnShip1.setNewCoords(spawnX,R+5);
        spawnShip1.changeSpeed(0,spawnShip1.getDefaultSpeedY());
        spawnShip1.setHealth(spawnShip1.getHoldHealth());
        spawnShip2.setNewCoords(fieldWidth-spawnX,R+5);
        spawnShip2.changeSpeed(0,spawnShip2.getDefaultSpeedY());
        spawnShip2.setHealth(spawnShip2.getHoldHealth());
        flyingEnemies.add(spawnShip1);
        flyingEnemies.add(spawnShip2);
        return true;
    }
}
