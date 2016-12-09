//
//  Definition.h
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#ifndef Definition_h
#define Definition_h

#define TRANSITION_TIME 0.5

//遅延して関数を呼ぶためのマクロ
#define call_after(callback, delay) \
runAction(Sequence::create(DelayTime::create(delay), CallFunc::create(callback), NULL))

#endif /* Definition_h */
