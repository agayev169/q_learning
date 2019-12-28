import gym
import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm

env = gym.make("CartPole-v0")

Q_TABLE_DISCRETE_STATES     = 20
LEARNING_RATE               = 0.01
DISCOUNT_FACTOR             = 0.90
PRINT_EVERY                 = 1000
STEPS_PER_EPISODE           = 200
EPISODES                    = 20000
LOSE_REWARD                 = -100
RANDOM_RATE                 = 0.5
RANDOM_START                = EPISODES // 2
RANDOM_END                  = EPISODES
RANDOM_DECAY                = RANDOM_RATE / (RANDOM_END - RANDOM_START)
RENDER                      = False

q_table = np.random.uniform(-5, 0, size=([Q_TABLE_DISCRETE_STATES] * env.observation_space.shape[0] + [env.action_space.n]))

low  = env.observation_space.low
high = env.observation_space.high

for i in range(len(low)): # velocities may be very big in magnitude, but are usually bounded
    if low[i] <= -10e10:
        low[i] = -5
    if high[i] >= 10e10:
        high[i] = 5


def discretize(observation):
    res = [0 for _ in range(len(observation))]

    for i in range(len(observation)):
        res[i] = int((observation[i] - low[i]) / (high[i] - low[i]) * Q_TABLE_DISCRETE_STATES)

        if res[i] >= Q_TABLE_DISCRETE_STATES:
            res[i] = Q_TABLE_DISCRETE_STATES - 1
        elif res[i] < 0:
            res[i] = 0

    return tuple(res)


def q_step(old_state, action, new_state, reward, lose, step):
    if lose:
        q_table[old_state + (action, )] = LOSE_REWARD
    else:
        q_table[old_state + (action, )] = (1 - LEARNING_RATE) * q_table[old_state + (action, )] + \
            (1 + step / STEPS_PER_EPISODE) * LEARNING_RATE * (reward + DISCOUNT_FACTOR * np.max(q_table[new_state]))


steps = 0
steps_history = []

for episode in tqdm(range(EPISODES)):
    done = False
    observation = env.reset()
    for step in range(STEPS_PER_EPISODE):
        if (episode + 1) % PRINT_EVERY == 0 and RENDER:
            env.render()
        if episode >= RANDOM_START and episode <= RANDOM_END and np.random.rand(1) < RANDOM_RATE:
            action = np.random.randint(0, env.action_space.n)
        else:
            action = np.argmax(q_table[discretize(observation)])

        if episode >= RANDOM_START and episode <= RANDOM_END:
            RANDOM_RATE -= RANDOM_RATE
        
        observation_new, reward, done, info = env.step(action)
        q_step(discretize(observation), action, discretize(observation_new), reward, done, step)        
        if done or step == STEPS_PER_EPISODE - 1:
            steps_history.append(step + 1)
            steps += (step + 1)
            break

        observation = observation_new

    if (episode + 1) % PRINT_EVERY == 0:
        print(f"Done in {steps / PRINT_EVERY:.3f} steps in average")
        steps = 0

env.close()

plt.plot(np.convolve(steps_history, [1/100] * 100, mode="valid"))
plt.xlabel("Episode")
plt.ylabel("Number of steps")
plt.show()