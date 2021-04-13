
import matplotlib.pyplot as plt
import numpy as np
import gym
import gym_minigrid
import time
import math

# graphic data:
steps, avg_returns, avg_lengths = [], [], []

# game data
directions = 4
player_positions = 256
key_positions = 256
door_states = 2
total_actions = 7
total_states = directions * player_positions * key_positions * door_states

# SARSA hyper-parameters
epsilon_constant = 0.5
alpha = 0.9
gamma = 0.2

class GameState:
    # 256 ^ 2 * 8 possible game states
    def __init__(self):
        self.dir = 0                # 0,1,2,3
        self.pos_p = 0              # [0:256]
        self.pos_k = 0              # [0:256]
        self.door_state = 0         # [0,1] (closed/open)

def random_policy(state):
    return state.action_space.sample()

# get state of game:
def get_game_state(env):
    gameState = GameState()
    gameState.dir = env.agent_dir
    gameState.pos_p = env.agent_pos[0] + env.agent_pos[1] * env.grid.height
    gameState.pos_k = 0

    # get keyp pos from env
    for i in range(len(env.grid.grid)):
        obj = env.grid.grid[i]
        if obj is not None:
            if isinstance(obj, gym_minigrid.minigrid.Key):
                gameState.pos_k = i
            if isinstance(obj, gym_minigrid.minigrid.Door):
                gameState.door_state = int(obj.is_open == True)
    if gameState.pos_k == 0:
        gameState.pos_k = gameState.pos_p

    return gameState


def get_move(env, myPolicy):
    gameState = get_game_state(env)
    action = myPolicy[gameState.dir][gameState.pos_p][gameState.pos_k][gameState.door_state]
    return action


# returns random action
def epsilon_greedy(state, Q, epsilon, N):

    d = state.dir
    p_pos = state.pos_p
    k_pos = state.pos_k
    d_s = state.door_state
    actions_values = Q[d][p_pos][k_pos][d_s]
    index_max = np.argmax(actions_values)
    max_actions = []
    low_value_actions = []
    actions = range(7)
    probability_density = []

    # get all max actions:
    for i in range(7):
        if abs(actions_values[index_max] - actions_values[i]) < 1e-03 and actions_values[index_max] > 1e-09:
            max_actions.append(i)
        else:
            low_value_actions.append(i)

    # compute actions probability distribution:
    for i in range(7):
        if i in max_actions:
            p = epsilon / 7 + (1 - epsilon)
        else:
            p = epsilon / 7
        probability_density.append(p)

    new_p_dens = []
    for elem in probability_density:
        if elem < 0:
            new_p_dens.append(0)
        else:
            new_p_dens.append(elem)
    new_p = [float(i) / sum(new_p_dens) for i in new_p_dens]

    picked_action = np.random.choice(a=actions, p=new_p)

    return picked_action


def boltzmann(state, Q, epsilon, N):

    d = state.dir
    p_pos = state.pos_p
    k_pos = state.pos_k
    d_s = state.door_state
    actions_values = Q[d][p_pos][k_pos][d_s]
    max_diff = 0

    for a1 in actions_values:
        for a2 in actions_values:
            diff = abs(a1 - a2)
            if diff > max_diff:
                max_diff = diff

    bs = math.log(N[d][p_pos][k_pos][d_s] + 1) / (max_diff + 1)

    b_sum = 0
    for a in actions_values:
        b_sum += math.exp(a * bs)

    prob_density = []
    for a in actions_values:
        prob_density.append(math.exp(a * bs) / (b_sum + 1))

    new_p = [float(i) / sum(prob_density) for i in prob_density]

    picked_action = np.random.choice(a=range(total_actions), p=new_p)

    return picked_action


def sarsa(policy):
    q_init_value = 0.5

    # Q(s,a) - State action value
    Q = np.full(total_states * total_actions, q_init_value)
    Q = Q.reshape((directions, player_positions, key_positions, door_states,
                                            total_actions))
    # N(s) - States visited nr
    N = np.zeros(total_states).reshape((directions, player_positions, key_positions, door_states))

    # P(s) - returned policy
    P = np.zeros(total_states).reshape((directions, player_positions, key_positions, door_states))

    nr_episodes = 400
    env = gym.make("MiniGrid-DoorKey-6x6-v0")

    for episode in range(1, nr_episodes):
        done = False
        env.seed()
        _obs = env.reset()

        old_state = get_game_state(env)

        visited_times = N[old_state.dir][old_state.pos_p][old_state.pos_k][old_state.door_state]
        epsilon = epsilon_constant / (visited_times + 1.0)
        action = policy(old_state, Q, epsilon, N)

        # policy evaluation frequency
        if episode % 20 == 0:
            print("Reached episode: %d" % episode)
            # Test update policy:
            test_policy(env, policy, Q, N)

        while not done:
            N[old_state.dir][old_state.pos_p][old_state.pos_k][old_state.door_state] += 1

            # execute action , get reward and new state
            next_obs, reward, done, _ = env.step(action)
            new_state = get_game_state(env)

            # apply e greedy for next action:
            visited_times = N[old_state.dir][old_state.pos_p][old_state.pos_k][old_state.door_state]
            epsilon = epsilon_constant / (visited_times + 1.0)
            next_action = policy(old_state, Q, epsilon, N)

            # update policy
            P[old_state.dir][old_state.pos_p][old_state.pos_k][old_state.door_state] = action

            # next_action = random_policy(env)
            qsa = Q[old_state.dir][old_state.pos_p][old_state.pos_k][old_state.door_state][action]
            next_qsa = Q[new_state.dir][new_state.pos_p][new_state.pos_k][new_state.door_state][next_action]

            # improve Q(s,a)
            new_qsa_value = qsa + alpha * (reward + gamma * next_qsa - qsa)

            Q[old_state.dir][old_state.pos_p][old_state.pos_k][old_state.door_state][action] = new_qsa_value

            old_state = new_state
            action = next_action


    return P


def test_policy(env, myPolicy, Q, N):

    env.seed()
    env.reset()

    nsteps = 16000
    report_freq = 2000

    recent_returns, recent_lengths = [], []
    crt_return, crt_length = 0, 0

    env.seed()
    _obs, done = env.reset(), False

    for step in range(1, nsteps + 1):

        game_state = get_game_state(env)
        epsilon = epsilon_constant / (N[game_state.dir][game_state.pos_p][game_state.pos_k][game_state.door_state] + 1)
        action = myPolicy(game_state, Q, epsilon, N)
        next_obs, reward, done, _ = env.step(action)

        crt_return += reward
        crt_length += 1

        #env.render("human")
        #print(action)
        #time.sleep(0.1)

        if done:
            env.seed()
            _obs, done = env.reset(), False

            recent_returns.append(crt_return)  # câștigul episodului încheiat
            recent_lengths.append(crt_length)
            crt_return, crt_length = 0, 0
        else:
            _obs = next_obs

        if step % report_freq == 0:
            avg_return = np.mean(recent_returns)  # media câștigurilor recente
            avg_length = np.mean(recent_lengths)  # media lungimilor episoadelor

            last_step_nr = 0
            if len(steps) == 0:
                last_step_nr = 0
            else:
                last_step_nr = steps[-1]
            steps.append(step + last_step_nr)  # pasul la care am reținut valorile
            avg_returns.append(avg_return)
            avg_lengths.append(avg_length)

            print(  # pylint: disable=bad-continuation
                f"Step {step:4d}"
                f" | Avg. return = {avg_return:.2f}"
                f" | Avg. ep. length: {avg_length:.2f}"
            )
            recent_returns.clear()
            recent_lengths.clear()


def main():
    # change SARSA policy: epsilon_greedy, boltzmann
    sarsa(epsilon_greedy)
    name="epsilon_greedy"

    # plot data collected:
    _fig, (ax1, ax2) = plt.subplots(ncols=2)
    ax1.plot(steps, avg_lengths, label=name)
    ax1.set_title("Avg. len. a=%f g=%f c=%f" % (alpha, gamma, epsilon_constant))
    ax1.legend()
    ax2.plot(steps, avg_returns, label=name)
    ax2.set_title("Avg. ret. a=%f g=%f c=%f" % (alpha, gamma, epsilon_constant))
    ax2.legend()
    plt.show()


if __name__ == "__main__":
    main()
