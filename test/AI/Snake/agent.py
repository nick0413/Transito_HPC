import torch as tr
import random as rd
import numpy as np
from game import SnakeGame, Direction, Point
from collections import deque


MAX_MEMORY = 100000
BATCH_SIZE = 1000
alpha = 0.001

class Agent:
	def __init__(self):
		self.number_of_games =0
		self.epsilon = 0		
		self.gamma = 0
		self.memory = deque(maxlen=MAX_MEMORY) 	# guarda elementos secuencialmente, es decir, si añade un elemento y ya se alcanzo el numero 
												# MAXIMO de elementos, remueve el mas viejo. Como la memoria a corto plazo real
		# TODO: modelo y trainer
		pass
	def get_state(self,game):
		pass
	def remember(self,state,action,reward,next_state,done):
		pass
	def train_long_memory(self):
		pass
	def train_short_memory(self,state,action,reward,next_state,done):
		pass
	def get_action(self,state):
		pass

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
				# TODO agent.model.save()
			print(f'Game {agent.number_of_games}, Score: {score}, Record: {record}')
			#TODO  ploting
	pass

if __name__ == '__main__':
	train()