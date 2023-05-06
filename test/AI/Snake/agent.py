import torch as tr
import random as rd
import numpy as np
from game import SnakeGame, Direction, Point
from collections import deque
from model import Linear_QNet, Qtrainer
from helper import plot
MAX_MEMORY = 100000
BATCH_SIZE = 1000
alpha = 0.001

class Agent:
	def __init__(self):
		self.number_of_games =0
		self.epsilon = 0		
		self.gamma = 0.9 
		self.memory = deque(maxlen=MAX_MEMORY) 	# guarda elementos secuencialmente, es decir, si añade un elemento y ya se alcanzo el numero 
												# MAXIMO de elementos, remueve el mas viejo. Como la memoria a corto plazo real
		self.model= Linear_QNet(11,256,3)
		self.trainer= Qtrainer(self.model,alpha,self.gamma)


	def get_state(self,game):
		head = game.snake[0]
		point_l = Point(head.x - 20,head.y)
		point_r = Point(head.x + 20,head.y)
		point_u = Point(head.x,head.y - 20)
		point_d = Point(head.x,head.y + 20)


		dir_l = (game.direction== Direction.LEFT)
		dir_r = (game.direction== Direction.RIGHT)
		dir_u = (game.direction== Direction.UP)
		dir_d = (game.direction== Direction.DOWN)

		 
		state =	[
					#al frente
					(dir_r and game.is_collision(point_r)) or
					(dir_l and game.is_collision(point_l)) or
					(dir_u and game.is_collision(point_u)) or
					(dir_d and game.is_collision(point_d)) ,
					#a la derecha
					(dir_u and game.is_collision(point_r)) or
					(dir_d and game.is_collision(point_l)) or
					(dir_l and game.is_collision(point_u)) or
					(dir_r and game.is_collision(point_d)) ,
					#a la izquierda
					(dir_d and game.is_collision(point_r)) or
					(dir_u and game.is_collision(point_l)) or
					(dir_r and game.is_collision(point_u)) or
					(dir_l and game.is_collision(point_d)) ,

					dir_l,
					dir_r,
					dir_u,
					dir_d,

					game.food.x<game.head.x,
					game.food.x>game.head.x,
					game.food.y>game.head.y,
					game.food.y<game.head.y

				]
		return np.array(state,dtype=int)
	

	def remember(self,state,action,reward,next_state,done):
		self.memory.append((state,action,reward,next_state,done))
	
	def train_long_memory(self):
		if len(self.memory) > BATCH_SIZE:
			sample = rd.sample(self.memory, BATCH_SIZE)

		else:
			sample = self.memory
		for i in range(len(self.memory)):
			print(len(self.memory[i]))
			print('\n')
		states,actions,rewards,next_states,dones = zip(*sample)

		self.trainer.train_step(states,actions,rewards,next_states,dones)

	def train_short_memory(self,state,action,reward,next_state,done):
		self.trainer.train_step(state,action,reward,next_state,done)

	def get_action(self,state):
		#exploration-explotation
		self.epsilon = 80 - self.number_of_games
		move = [0,0,0]
		if rd.randint(0,200) < self.epsilon:
			move_idx= rd.randint(0,2)
			move[move_idx]=1
		else:
			state0 = tr.tensor(state,dtype= tr.float)
			prediction= self.model(state0)
			move_idx = tr.argmax(prediction).item()
			move[move_idx]=1

		return move

		

def train():
	plot_scores= []
	plot_mean_scores =[]
	total_score=0
	record = 0
	agent = Agent()
	game = SnakeGame()

	while True:
		state_old = agent.get_state(game)

		move= agent.get_action(state_old)

		reward,done,score=game.play_step(move)

		state_new = agent.get_state(game)
		
		agent.train_short_memory(state_old,move,reward,state_new,done)

		agent.remember(state_old,move,reward,state_new,done)

		if done:
			#experience replay || replay memory
			#update plot

			game.reset()
			agent.number_of_games += 1
			agent.train_long_memory()

			if score> record:
				record=score
				agent.model.save_model()
			print(f'Game {agent.number_of_games}, Score: {score}, Record: {record}')
			
		plot_scores.append(score)
		size=10
		last_totals=sum(plot_scores[len(plot_scores)-1-size:len(plot_scores)-1])/size
	
		plot_mean_scores.append(last_totals)
		plot(plot_scores,plot_mean_scores)

if __name__ == '__main__':
	train()