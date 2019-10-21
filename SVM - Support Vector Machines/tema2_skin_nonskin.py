import sys

from svmutil import *
from sklearn.metrics import confusion_matrix
import numpy as np
import matplotlib.pyplot as plt

from sklearn import svm, datasets
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix
from sklearn.utils.multiclass import unique_labels


#1 skin
#2 nonskin

def plot_confusion_matrix(y_true, y_pred, classes,
                          normalize=False,
                          title=None,
                          cmap=plt.cm.Blues):
    """
    This function prints and plots the confusion matrix.
    Normalization can be applied by setting `normalize=True`.
    """
    if not title:
        title = 'Confusion matrix, without normalization'

    # Compute confusion matrix
    cm = confusion_matrix(y_true, y_pred)
    
    print('Confusion matrix, without normalization')

    print(cm)

    fig, ax = plt.subplots()
    im = ax.imshow(cm, interpolation='nearest', cmap=cmap)
    ax.figure.colorbar(im, ax=ax)
    # We want to show all ticks...
    ax.set(xticks=np.arange(cm.shape[1]),
           yticks=np.arange(cm.shape[0]),
           # ... and label them with the respective list entries
           xticklabels=classes, yticklabels=classes,
           title=title,
           ylabel='True label',
           xlabel='Predicted label')

    # Rotate the tick labels and set their alignment.
    plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
             rotation_mode="anchor")

    # Loop over data dimensions and create text annotations.
    fmt = '.2f' if normalize else 'd'
    thresh = cm.max() / 2.
    for i in range(cm.shape[0]):
        for j in range(cm.shape[1]):
            ax.text(j, i, format(cm[i, j], fmt),
                    ha="center", va="center",
                    color="white" if cm[i, j] > thresh else "black")
    fig.tight_layout()
    return ax

def scale_skin_no_skin(features):
    scaled_features = []
    for feature in features:
      feature_dict = {}
      for key in feature:
        feature_dict[key] = feature[key] / 255
      scaled_features.append(feature_dict)
    return scaled_features

y_train, x_train = svm_read_problem('training_skin_nonskin')
# SCALE FEATURES:
scaled_x_train = scale_skin_no_skin(x_train)
prob = svm_problem(y_train, scaled_x_train)

for r in range(1, 15):
  param_str = '-s 1 -t 3 -c 0.5 -g 0.1 -r %f' % (r * 0.2)
  param = svm_parameter(param_str)
  print(param)

  model = svm_train(prob, param)

  y_test, x_test = svm_read_problem('testing_skin_nonskin')
  # scale data!:
  scaled_x_test = scale_skin_no_skin(x_test)
  p_label_test, p_acc, p_val = svm_predict(y_test, scaled_x_test, model)

  nr_sv = model.get_nr_sv()
  print("nr_sv")
  print(nr_sv)

  print(p_acc)

  p_label_train, p_acc_tr, p_val_tr = svm_predict(y_train, scaled_x_train, model)

  np.set_printoptions(precision=2)

  class_names = ['skin', 'nonskin']
  acc_string = '_acc=' + str(p_acc[0])
  # Plot non-normalized confusion matrix
  plot_confusion_matrix(y_test, p_label_test, classes=class_names,
                        title='Matrice de confuzie - set de testare')

  plt.savefig("sigmoid/coef/" + param_str + acc_string + '_test.png')


  plot_confusion_matrix(y_train, p_label_train, classes=class_names,
                        title='Matrice de confuzie - set de antrenare')



  plt.savefig("sigmoid/coef/" + param_str + '_train.png')