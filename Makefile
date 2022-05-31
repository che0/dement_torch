program: dement_torch

dement_torch: dement_torch.c
	cc -W -Wall -o $@ $< -lpthread -lcurl
