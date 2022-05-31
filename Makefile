program: dement_torch

dement_torch: dement_torch.c
	cc -W -Wall -lpthread -lcurl -o $@ $< 
