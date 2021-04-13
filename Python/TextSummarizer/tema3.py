#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re
import csv
import string
import nltk
from math import log
from nltk.stem import WordNetLemmatizer
from nltk.tokenize import sent_tokenize
nltk.download('wordnet')
nltk.download('averaged_perceptron_tagger')
nltk.download('punkt')

#receives the original text and returns the preprocessed text
def clean_text(text):
	stop_words = [line.strip() for line in open("Lab12-stop_words")]
	text = re.sub(r'\d+', '', text)		#delete digits
	text = re.sub(r'[^a-zA-Z]', ' ', text)		#replace special characters with a space
	text = re.sub(r'(\s[a-zA-Z]{1,2}\s)', ' ', text)		#replace 1-letter and 2-letter words with spaces
	text = re.sub(r'(\s[a-zA-Z]{1,2}\s)', ' ', text)
	text = [word + " " for word in text.split() if word.lower() not in stop_words]
	new_text = ' '.join(word for word in text)
	return new_text

#parameter: an already processed text, where the words are delimited by spaces
#return value: list of lemmatized words
def lemmatize(preprocessed_text):
	wordnet_lemmatizer = WordNetLemmatizer()
	to_ret = []
	text_as_list = preprocessed_text.split()

	for word in text_as_list:
		new_word = wordnet_lemmatizer.lemmatize(word)
		to_ret.append(new_word)
	return to_ret

#parameter: a list of words
#return value: list of nouns
def ret_nouns_list(word_list):
	to_ret = []
	
	tagged = nltk.pos_tag(word_list)
	for tup in tagged:
		if tup[1] in ["NN", "NNS", "NNP", "NNPS"]:		#NNS is taken into account because of examples like this ('Wednesdays', 'NNS');('Times', 'NNS')
			to_ret.append(tup[0])
	return to_ret

def get_tf_dict(word_list):
	tf_dict = {}

	for word in word_list:
		if word in tf_dict.keys():
			pass
		else:
			cnt = word_list.count(word)
			tf_dict.update({word:cnt})
	return tf_dict


def get_idf_dict(dict_list, valid_articles_cnt):
	idf_dict = {}

	#firstly, the idf dictionary will contain only the nr of articles containing the word 
	for dictionary in dict_list:
		if dictionary:
			for key, value in dictionary.items():
				if key in idf_dict.keys():
					new_val = idf_dict[key] + 1
					idf_dict.update({key:new_val})
				else:
					new_val = 1
					idf_dict.update({key:new_val})

	#in the end, the idf dictionary will contain the idf for each word
	for key, value in idf_dict.items():
		new_value = log(valid_articles_cnt/value)
		idf_dict.update({key:new_value})
	return idf_dict

def get_tf_idf_dict(tf_dict, idf_dict):
	tf_idf_dict = {}

	for key, tf_value in tf_dict.items():
		idf_val = idf_dict[key]
		to_insert = tf_value * idf_val
		tf_idf_dict.update({key:to_insert})
	return tf_idf_dict

#clean extra punctuation marks in order to prepare the text to be splitted into sentences
def clean_text_before_splitting(text):
	text = re.sub('\.([a-z,]+)',r'\1', text)	#remove apparent sentence delimiters(cases where ? replaces "")
	text = re.sub('\? \?', ' ', text)			#remove '? ?' because it was supposed to be '" "'
	text = re.sub('[?!]([a-zA-Z,]+| [a-z]+)',r"\1", text)	#remove apparent sentence delimiters(cases where ? replaces ')
	text = re.sub('([.])([^.]+)',r'\1 \2', text)	#add a space after '.' for tokenizing the text into more sentences
	text = re.sub('([?])([^?]+)',r'\1 \2', text)
	return text

def get_headline_score(sentence_words, headline):		#get the headline score based on all words, not just nouns
	parsed_headline = clean_text(headline)
	headline_words = lemmatize(parsed_headline)

	headline_score = 0
	for word in headline_words:
		if word in sentence_words:
			headline_score = headline_score + 1

	headline_score = headline_score / len(headline_words)
	headline_score = headline_score * 0.5
	return headline_score

def get_sentence_position_score(valid_sentences_list, sentence_index):
	return sentence_index / len(valid_sentences_list)

def get_best_three(valid_sentences_list, sentences_score_list):
	best_sentences = []

	nr_sentences_in_summarization = min(len(sentences_score_list) - 1, 2)		#if the text contains less than 3 sentences

	for i in range(nr_sentences_in_summarization):
		max_score = max(sentences_score_list)
		max_score_index = sentences_score_list.index(max_score)
		best_sentences.append(valid_sentences_list[max_score_index])
		sentences_score_list[max_score_index] = -1

	return best_sentences

def get_sentences_by_score(article_text, tf_idf_table, headline):
	if not tf_idf_table:					#if the article text is invalid
		return []
	
	text = clean_text_before_splitting(article_text)
	sentences_list = sent_tokenize(text)		#break the article text into sentences
	valid_sentences = []
	sentences_score_list = []

	for sentence in sentences_list:
		text = clean_text(sentence)				#get the preprocessed text
		sentence_words = lemmatize(text)
		sentence_nouns = ret_nouns_list(sentence_words)

		sentence_score = 0
		if len(sentence_words) < 2:				#don't take into consideration sentences with less than two words
			pass
		else:
			valid_sentences.append(sentence)	#create a list containing only valid sentences (longer than 1 word)

			for noun in sentence_nouns:
				if noun in tf_idf_table.keys():
					sentence_score = sentence_score + tf_idf_table[noun]

			sentence_score = sentence_score / len(sentence_words)				#normalize the score
			
			headline_score = get_headline_score(sentence_words, headline)		#get the score for headline resemblance
			sentence_score = sentence_score + headline_score

			sentences_score_list.append(sentence_score)

	for i in range(len(valid_sentences)):
		sentence_pos_score = get_sentence_position_score(valid_sentences, i)		#get sentence position score
		sentences_score_list[i] = sentences_score_list[i] + sentence_pos_score
		
	return get_best_three(valid_sentences, sentences_score_list)

def generate_n_grams(n, text):
	lower_text_as_list = [str.lower(letter) if letter.isalpha() == True else letter for letter in text]	     #replace all uppercase chars with lowercase
	lower_str = ''.join(lower_text_as_list)

	parsed_str = re.sub('[^a-zA-Z ]', ' ', lower_str)														#replace all but alphabetical characters
	words = parsed_str.split()

	n_grams = [words[i:i + n] for i in range(len(words) - (n - 1))]											#get n_grams as lists of words
	n_grams_str_list = [' '.join(n_gram_list) for n_gram_list in n_grams]									#get n_grams as strings
	return n_grams_str_list

#parameter: two lists of strings (each string represents an n-gram)
#return value: number of common n-grams
def cnt_common_ngrams(n_gram_1, n_gram2):
	cnt = 0

	for n_gram in n_gram_1:
		if n_gram in n_gram2:
			cnt = cnt + 1
	return cnt


with open('dataset_tema3.csv', 'r') as csvFile:
	reader = csv.reader(csvFile)
	data_list = list(reader)
	data_list = data_list[1:]


	all_articles_tf_dicts = []
	valid_articles_cnt = 0					#number of articles containing valid text
	empty_dict = {}

	for i in range(len(data_list)):
		text = data_list[i][2]				#get the ctext field
		text = clean_text(text)				#get the preprocessed text
		article_words = lemmatize(text)
		nouns = ret_nouns_list(article_words)
		if not nouns:
			all_articles_tf_dicts.append(empty_dict)	
		else:
			valid_articles_cnt = valid_articles_cnt + 1
			tf_dict = get_tf_dict(nouns)					#get the tf table for all nouns in text
			all_articles_tf_dicts.append(tf_dict)			#append to the list of tf dictionaries
	idf_dict = get_idf_dict(all_articles_tf_dicts, valid_articles_cnt)		

	all_articles_tf_idf_dicts = []
	for tf_dict in all_articles_tf_dicts:
		tf_idf_dict =  get_tf_idf_dict(tf_dict, idf_dict)
		all_articles_tf_idf_dicts.append(tf_idf_dict)

	my_summarizations = []

	len_my_summ_1_gram = 1
	len_my_summ_2_gram = 1
	len_my_summ_4_gram = 1

	bleu_1_sum = 0
	bleu_2_sum = 0
	bleu_4_sum = 0

	rouge_1_sum = 0
	rouge_2_sum = 0
	rouge_4_sum = 0

	article_cnt = 0

	for i in range(len(data_list)):
		text = data_list[i][2]
		ground_truth = data_list[i][1]
		title = data_list[i][0]

		summarization = get_sentences_by_score(text, all_articles_tf_idf_dicts[i], title)
		if not summarization or len(summarization) == 0:					#for articles with invalid text
			pass
		else:
			article_cnt = article_cnt + 1
			summ_text = ' '.join(summarization)						#summarized text

			my_summ_1_gram = generate_n_grams(1, summ_text)			#get the 1-gram list for the computed summarization
			my_summ_2_gram = generate_n_grams(2, summ_text)
			my_summ_4_gram = generate_n_grams(4, summ_text)

			ground_truth_1_gram = generate_n_grams(1, ground_truth)
			ground_truth_2_gram = generate_n_grams(2, ground_truth)
			ground_truth_4_gram = generate_n_grams(4, ground_truth)

			cnt_common_1grams = cnt_common_ngrams(my_summ_1_gram, ground_truth_1_gram) 
			cnt_common_2grams = cnt_common_ngrams(my_summ_2_gram, ground_truth_2_gram) 
			cnt_common_4grams = cnt_common_ngrams(my_summ_4_gram, ground_truth_4_gram) 

			if len(my_summ_1_gram) != 0:
				len_my_summ_1_gram = len(my_summ_1_gram)
			
			if len(my_summ_2_gram) != 0:
				len_my_summ_2_gram = len(my_summ_2_gram)

			if len(my_summ_4_gram) != 0:
				len_my_summ_4_gram = len(my_summ_4_gram)


			bleu_1 = cnt_common_1grams / len_my_summ_1_gram
			bleu_2 = cnt_common_2grams / len_my_summ_2_gram
			bleu_4 = cnt_common_4grams / len_my_summ_4_gram

			rouge_1 = cnt_common_1grams / len(ground_truth_1_gram)
			rouge_2 = cnt_common_2grams / len(ground_truth_2_gram)
			rouge_4 = cnt_common_4grams / len(ground_truth_4_gram)

			bleu_1_sum = bleu_1_sum + bleu_1
			bleu_2_sum = bleu_2_sum + bleu_2
			bleu_4_sum = bleu_4_sum + bleu_4

			rouge_1_sum = rouge_1_sum + rouge_1
			rouge_2_sum = rouge_2_sum + rouge_2
			rouge_4_sum = rouge_4_sum + rouge_4

	bleu_1_med = bleu_1_sum / article_cnt
	bleu_2_med = bleu_2_sum / article_cnt
	bleu_4_med = bleu_4_sum / article_cnt

	rouge_1_med = rouge_1_sum / article_cnt
	rouge_2_med = rouge_2_sum / article_cnt
	rouge_4_med = rouge_4_sum / article_cnt

	print("----bleu_@1_med------")
	print(bleu_1_med)

	print("----bleu_@2_med------")
	print(bleu_2_med)
	print("----bleu_@4_med------")
	print(bleu_4_med)

	print("----rouge_@1_med------")
	print(rouge_1_med)

	print("----rouge_@2_med------")
	print(rouge_2_med)
	print("----rouge_@4_med------")
	print(rouge_4_med)