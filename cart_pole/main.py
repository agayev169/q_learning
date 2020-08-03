import gym
import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm

env = gym.make("CartPole-v0")

Q_TABLE_DISCRETE_STATES     = 40
LEARNING_RATE               = 0.1
DISCOUNT_FACTOR             = 0.99
PRINT_EVERY                 = 200
STEPS_PER_EPISODE           = 200
EPISODES                    = 50000
RANDOM_RATE                 = 0.95
RANDOM_START                = 0
RANDOM_RATE_MIN             = 0.1
RANDOM_DECAY                = 0.99999
RENDER                      = False

q_table = np.random.uniform(0, 1, size=([Q_TABLE_DISCRETE_STATES] * env.observation_space.shape[0] + [env.action_space.n]))

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
        q_table[old_state + (action, )] = (1 - LEARNING_RATE) * q_table[old_state + (action, )] + LEARNING_RATE * reward
    else:
        q_table[old_state + (action, )] = (1 - LEARNING_RATE) * q_table[old_state + (action, )] \
            + LEARNING_RATE * (reward + DISCOUNT_FACTOR * np.max(q_table[new_state]))

def test(env, n):
    steps = []
    for step in range(n):
        observation = env.reset()
        for step in range(STEPS_PER_EPISODE):
            action = np.argmax(q_table[discretize(observation)])
            observation, reward, done, _ = env.step(action)
            if done or step == STEPS_PER_EPISODE:
                steps.append(step + 1)
                break
    return steps

steps = 0
steps_history = []
test_steps_history = []

test_steps = []
pbar = tqdm(range(EPISODES))
for episode in pbar:
    done = False
    observation = env.reset()
    for step in range(STEPS_PER_EPISODE):
        if (episode + 1) % PRINT_EVERY == 0 and RENDER:
            env.render()
        if episode >= RANDOM_START and np.random.rand(1) < RANDOM_RATE:
            action = np.random.randint(0, env.action_space.n)
        else:
            action = np.argmax(q_table[discretize(observation)])

        if episode >= RANDOM_START and RANDOM_RATE > RANDOM_RATE_MIN:
            RANDOM_RATE *= RANDOM_DECAY
        
        observation_new, reward, done, info = env.step(action)
        q_step(discretize(observation), action, discretize(observation_new), reward, done, step)        
        if done or step == STEPS_PER_EPISODE - 1:
            steps_history.append(step + 1)
            steps += (step + 1)
            break

        observation = observation_new

    if (episode + 1) % PRINT_EVERY == 0:
        test_steps = test(env, PRINT_EVERY)
        test_steps_history.extend(test_steps)
        if sum(test_steps) / PRINT_EVERY > 199:
            print('Done')
            break
    pbar_desc = f"Avg steps: test: {sum(test_steps) / PRINT_EVERY}; train: {sum(steps_history[-100:]) / 100.0} - random rate: {RANDOM_RATE:.4f}"
    pbar.set_description(pbar_desc)
    # if (episode + 1) % PRINT_EVERY == 0:
    #     print(f"Done in {steps / PRINT_EVERY:.3f} steps in average")
    #     steps = 0

env.close()

plt.plot(np.convolve(steps_history, [1/100] * 100, mode="valid"), label='train')
plt.plot(np.convolve(test_steps_history, [1/100] * 100, mode="valid"), label='test')
plt.xlabel("Episode")
plt.ylabel("Number of steps")
plt.legend()
plt.show()
