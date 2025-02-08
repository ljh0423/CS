# (C) Jeff Orchard, 2025
# University of Waterloo

import numpy as np
from neuron_models import *


#================================
#
#  LIFNeuron class
#
#================================
class LIFNeuron(SpikingNeuron):

    def __init__(self, Tau_m=0.02, Tau_ref=0.002, Tau_s=0.05, label=None):
        '''
         lif = LIFNeuron(Tau_m=0.02, Tau_ref=0.002, Tau_s=0.05, label=None)
        
         Constructor for LIFNeuron class
        
         Inputs:
           Tau_m   membrane time constant, in seconds (s)
           Tau_ref refractory period (s)
           Tau_s   synaptic time constant (s)
           label   optional string
        '''
        # The base class SpikingNeuron sets:
        #   self.t
        #   self.label
        #   self.__spikes
        #   self.axon
        #   self.__Incoming_spikes_buffer
        super().__init__(label=label)

        self.tau_m = Tau_m      # membrane time constant
        self.tau_ref = Tau_ref  # refractory period
        self.tau_s = Tau_s      # synaptic time constant

        self.__v = 0.             # sub-threshold membrane potential (voltage)
        self.__s = 0.             # post-synaptic current (PSC)
        self.__dvdt = 0.          # rate of change of v
        self.__dsdt = 0.          # rate of change of s
        
        self.__ref_remaining = 0. # amount of time remaining in the refractory period
                
        # During a time step, spikes sent out from neurons are received by other neurons,
        # incrementing the receiving neuron's incoming_spikes_buffer.
        # When slope is called, that buffer is copied to self.__I so that it can be used
        # by the next call to step. That is, self.__I appears in the RHS of the
        # differential equation for s.
        self.__I = 0.             # current injected by arriving spikes
        
        # For plotting
        self.__v_history = []     # records v over time
        self.__s_history = []     # records s over time
    
        
    def slope(self):
        '''
         lif.slope()
        
         Evaluates the right-hand side of the differential equations that
         govern v and s. This function does NOT include incoming spikes.
         
         There is no output; the method simply updates the member variables
            self.__dvdt, and
            self.__dsdt
         which are used by the step method.
        '''
        # Copy the input spikes buffer, then reset it for the
        # upcoming "step" process. We do this because the step process
        # increments the spikes buffer for neurons that
        # receive spikes.
        self.__I = self.get_input_buffer()
        self.set_input_buffer(0.)
        
        #=========== YOUR CODE (start) ===========
        # (a)
        # YOUR CODE HERE
                    
        self.__dvdt = (self.__s - self.__v) / self.tau_m
        self.__dsdt = (-self.__s) / self.tau_s
        #=========== YOUR CODE (end) ===========

        
    def step(self, dt):
        '''
         lif.step(dt)
        
         Updates the LIF neuron state by taking an Euler step in v and s.
         The length of the step is dt seconds.
         
         Input
          dt       time step (in seconds)
          
         If v reaches the threshold of 1, the neuron fires an action potential
         (spike). Linear interpolation is used to estimate the time that v=1.
         The spike time is appended to the list self.__spikes, and v
         is set to zero. After a spike, the neuron is dormant for self.tau_ref
         seconds.
        '''
        # self.__I includes all the input from spikes in the last time step
        
        # Store v (for plotting)
        self.__v_history.append(self.__v)
        self.__s_history.append(self.__s)
        
        #=========== YOUR CODE (start) ===========
        # (b)
        # YOUR CODE HERE

        # Update the synaptic current using Euler's method
        self.__s += self.__dsdt * dt

        # When spike arrives in self.__I
        self.__s += self.__I / self.tau_s
        
        # If in ref period
        if self.__ref_remaining > 0:
            self.__v = 0.0  # Keep membrane potential at 0 during refractory period
            if self.__ref_remaining < dt:
                self.t += self.__ref_remaining # set time to when ref period ends
                dt -= self.__ref_remaining # set dt to time left after ref period ends
                self.__ref_remaining = 0.0
            else:
                self.__ref_remaining -= dt
                return

        # Update the membrane potential using Euler's method
        self.__v += self.__dvdt * dt

        # Check if the membrane potential has exceeded the threshold
        if self.__v >= 1.0:  # Spike occurs
            # Use linear interpolation to estimate the spike time
            t_spike = self.t - (self.__v - 1.0) / self.__dvdt + dt
            self.add_spike(t_spike)  # Record the spike time
            self.__v = 1.0  # set membrane potential
            self.__ref_remaining = self.tau_ref  # Start the refractory period
            self.send_spike()  # Send the spike to downstream neurons

        # To add a spike at time self.t, use
        # self.add_spike(self.t)
        #=========== YOUR CODE (end) ===========

        self.t += dt     # step forward in time
            
    def set_v(self, v):
        self.__v = v
    def get_v(self):
        return self.__v
    def set_s(self, s):
        self.__s = s
    def get_s(self):
        return self.__s
    def get_s_history(self):
        return self.__s_history
    def get_v_history(self):
        return self.__v_history
    
    def __repr__(self):
        '''
         lif.print(neur)
         
         Prints the current time, membrane potential, input current, and
         remaining refractory time.
        '''
        s = f'{self.t:6.4f}s: s={self.__s:5.3f}, v={self.__v:6.4f}, ref remaining={self.__ref_remaining:7.5f}s'
        return s
    
