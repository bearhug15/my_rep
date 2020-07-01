package game;

import moving_objects.MovingObject;

import java.util.*;

public interface MyObserver {

    void update(AbstractCollection<? extends AbstractCollection<? extends MovingObject>> data);

}

