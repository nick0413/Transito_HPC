import torch as tr
import torch.nn as nn
import torch.nn.functional as fn
import torch.optim as optim
import os

class Linear_QNet(nn.Module):
	def __init__(self, input,hidden, output):
		super().__init__()
		self.linear1=nn.Linear(input,hidden)
		self.linear2=nn.Linear(hidden,output)
		tr.autograd.set_detect_anomaly(True)
	def forward(self,x):
		x =fn.relu(self.linear1(x))
		x = self.linear2(x)
		return x
	
	def save_model(self,file_name='Snake_model.pth'):
		model_folder = './modelo'
		if not os.path.exists(model_folder):
			os.makedirs(model_folder)

		path=os.path.join(model_folder,file_name)

		tr.save(self.state_dict(),path)
		

class Qtrainer:
	def __init__(self,model,alpha,gamma):
		self.alpha = alpha
		self.gamma=gamma
		self.model=model
		self.optimizer=optim.Adam(model.parameters(), lr=self.alpha)

		self.loss_fun =nn.MSELoss()		#funcion de costo de minimos cuadrados  E=(Q-Q_new)**2

	def train_step(self,state_old,move,reward,state_new,done):
		state=tr.tensor(state_old,dtype=tr.float)
		state_new= tr.tensor(state_new,dtype=tr.float)
		action = tr.tensor(move,dtype=tr.long)
		reward = tr.tensor(reward,dtype=tr.float)

		if len(state.shape)==1: # cambiamos los tensores de dimension (x) a (1,x), es como reshape de numpy
								# esto solo lo hacemos si es de 1 elemento porq si fuera de n, entonces el tensor 
								# ya estaria en la forma que queremos
			state =tr.unsqueeze(state,0)
			state_new =tr.unsqueeze(state_new,0)
			action =tr.unsqueeze(action,0)
			reward =tr.unsqueeze(reward,0)
			done=(done,)#tupla con un solo valor

		'''
		Segun la version simplificada de la ecucacion de bellman
		Tenemos que Q debe ser igual al resultado del modelo para el estado_0
		Y el nuevo Q debe Ser el interior de la estimacionde de Q*

		es decir

		Q=model.precit(state_0)
		Q_new=R_0+\gamma*max(Q(state_1))

		Donde state_0 y state_1 son estados del sistema a un \Delta t de distancia
		'''

		# Saber q con el estado actual
		pred = self.model(state)
		


		'''
		1.	Saber q_new= R + gamma * max(next_q) solo hacer esto si no game over 
		2.	pred.clone()
		3.	preds[argmax(action)]=q_new

		Esto lo que hace es mirar al futuro un tiempo 
		Basado en el Q del futuro dada la accion actual, cual es el Q del futuro

		Es decir. Si asumimos explotacion, entonces la accion que se va a tomar es argmax(action)
		en ese caso, actualizamos el valor del Q actual basado en sus consecuencias mirando un paso al futuro.
		'''

		target = pred.clone()
		for idx in range(len(done)):
			print('paso-----------',idx,'veces')
			Q_new = reward[idx]
			if not done[idx]:
				Q_new= reward[idx] +self.gamma*tr.max(self.model(state_new[idx]))

			target[idx][tr.argmax(action)].item =Q_new

			self.optimizer.zero_grad() #empty the gradient || pytorch

			loss = self.loss_fun(target,pred)
			loss.backward(retain_graph=True)

			self.optimizer.step()


		






		