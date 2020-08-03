import gym
import numpy as np
from tqdm import tqdm
import matplotlib.pyplot as plt

class Agent:
    def __init__(self, lr=0.1, discount_factor=0.95):
        self.observations_n = env.observation_space.n
        self.actions_n = env.action_space.n
        self.q_vals = np.random.rand(self.observations_n, self.actions_n)
        self.lr = lr
        self.discount_factor = discount_factor

    def choose_action(self, state, epsilon):
        if np.random.random() < epsilon:
            return np.random.randint(0, self.actions_n)
        else:
            return np.argmax(self.q_vals[state])
    
    def update(self, state, action, reward, new_state, done):
        new_action = self.choose_action(new_state, 0)
        if not done:
            self.q_vals[(state, action)] += self.lr * (reward + self.discount_factor 
                                            * self.q_vals[(new_state, new_action)] - self.q_vals[(state, action)])
        if done:
            self.q_vals[(state, action)] += self.lr * (reward - self.q_vals[(state, action)])

def test(N, agent, env):
    mean_reward = 0
    for _ in range(N):
        state = env.reset()
        episode_reward = 0
        done = False
        while not done:
            action = agent.choose_action(state, 0)
            new_state, reward, done, _ = env.step(action)
            episode_reward += reward
            state = new_state
        mean_reward += episode_reward
    return mean_reward / N

EPSILON_START = 1.0
EPSILON_FINAL = 0.1
EPSILON_STEPS = 100_000

LEARNING_RATE = 0.1
DISCOUNT_FACTOR = 0.95

TOTAL_ITERATIONS = 1_000_000

env = gym.make('FrozenLake-v0')
agent = Agent(lr=LEARNING_RATE)

reward_history = []
mean_reward = 0
best_mean_reward = 0

episode = 0
episode_reward = 0
state = env.reset()
pbar = tqdm(range(TOTAL_ITERATIONS))
for it in pbar:
    epsilon = max(EPSILON_FINAL, EPSILON_START - (EPSILON_START - EPSILON_FINAL) * it / EPSILON_STEPS)
    action = agent.choose_action(state, epsilon)
    new_state, reward, done, _ = env.step(action)
    episode_reward += reward
    agent.update(state, action, reward, new_state, done)
    if done:
        state = env.reset()
        episode += 1
        reward_history.append(episode_reward)
        episode_reward = 0

        # cur_mean_reward = np.mean(reward_history[-100:])
        # if cur_mean_reward > mean_reward:
        #     mean_reward = cur_mean_reward
        if (episode + 1) % 100 == 0:
            mean_reward = test(100, agent, env)
            if mean_reward > best_mean_reward:
                best_mean_reward = mean_reward
    else:
        state = new_state
    
    pbar.set_description(f"epsilon: {epsilon:.2f} - mean_reward: {mean_reward} - best_mean_reward: {best_mean_reward}")