package spawn_scenario;

import moving_objects.EnemyShip;

import java.util.LinkedList;

public class DifSpeedScenario implements Scenario {
    @Override
    public boolean spawn(LinkedList<EnemyShip> holdEnemies, LinkedList<EnemyShip> flyingEnemies, int fieldWidth) {
        if(holdEnemies.size()<4){
            return false;
        }
        EnemyShip spawnShip1 = holdEnemies.removeFirst();
        EnemyShip spawnShip2 = holdEnemies.removeFirst();
        EnemyShip spawnShip3 = holdEnemies.removeFirst();
        EnemyShip spawnShip4 = holdEnemies.removeFirst();
        int R = spawnShip1.getR();
        if(fieldWidth -4*R<0){
            throw new RuntimeException("Your screen is too small");
        }
        int spawnX = (int)(Math.random()*(fieldWidth - 2*R)) +R;
        if(spawnX<fieldWidth/2+R && spawnX>fieldWidth/2-R){
            spawnX-=R/2;
        }
        spawnShip1.setNewCoords(spawnX,R+5);
        spawnShip1.changeSpeed(0,(int)(spawnShip1.getDefaultSpeedY()*1.5));
        spawnShip1.setHealth(spawnShip1.getHoldHealth());
        spawnShip2.setNewCoords(fieldWidth-spawnX,R+5);
        spawnShip2.changeSpeed(0,(int)(spawnShip2.getDefaultSpeedY()*1.5));
        spawnShip2.setHealth(spawnShip2.getHoldHealth());
        spawnShip3.setNewCoords(spawnX,R+5);
        spawnShip3.changeSpeed(spawnShip3.getDefaultSpeedX(),(int)(spawnShip3.getDefaultSpeedY()*1.5));
        spawnShip3.setHealth(spawnShip3.getHoldHealth());
        spawnShip4.setNewCoords(fieldWidth-spawnX,R+5);
        spawnShip4.changeSpeed(-spawnShip4.getDefaultSpeedX(),(int)(spawnShip4.getDefaultSpeedY()*1.5));
        spawnShip4.setHealth(spawnShip4.getHoldHealth());
        flyingEnemies.add(spawnShip1);
        flyingEnemies.add(spawnShip2);
        flyingEnemies.add(spawnShip3);
        flyingEnemies.add(spawnShip4);
        return true;
    }
}
