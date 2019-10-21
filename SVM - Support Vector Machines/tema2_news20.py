import sys

from svmutil import *
from sklearn.metrics import confusion_matrix
import numpy as np
import matplotlib.pyplot as plt

from sklearn import svm, datasets
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix
from sklearn.utils.multiclass import unique_labels

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
    # Only use the labels that appear in the data
    
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
    return cm

def get_one_to_many_y(y, class_index):
  # only two labels remain:
  # 1 - is current class
  # 2 - not current class (other classes)
  class_y = []
  for label in y:
    if int(round(label)) == int(round(class_index)):
      class_y.append(1.0)
    else:
      class_y.append(2.0)
  return class_y

class ClassModel:
  def __init__(self, class_id, model, y_train, x_train):
        self.model = model
        self.class_id = class_id
        # data used for training:
        self.y_train = y_train
        self.x_train = x_train

        # testing results
        self.test_predictions = []
        self.test_prob = []

        # testing on training data:
        self.train_predictions = []
        self.train_prob = []


def combine_train_predictions(test_size, class_models):
  combined_predictions = []
  for i in range(test_size):
    max_prob = 0
    class_predicted = 0
    # search for highest confidence:
    for class_model in class_models:
      if class_model.train_prob[i][1] > max_prob:
        # model predicted by this class is the same
        # as model's class
        if class_model.train_predictions[i] == 1.0:
          max_prob = class_model.train_prob[i][1]
          class_predicted = class_model.class_id
    combined_predictions.append(class_predicted)
  return combined_predictions

def combine_test_predictions(test_size, class_models):
  combined_predictions = []
  for i in range(test_size):
    max_prob = 0
    class_predicted = 0
    # search for highest confidence:
    for class_model in class_models:
      if class_model.test_prob[i][1] > max_prob:
        # model predicted by this class is the same
        # as model's class
        if class_model.test_predictions[i] == 1.0:
          max_prob = class_model.test_prob[i][1]
          class_predicted = class_model.class_id
    combined_predictions.append(class_predicted)
  return combined_predictions

# CHANGE THIS vvv:
used_samples_nr = 500

y_train, x_train = svm_read_problem('news20.scale')
y_train = y_train[:used_samples_nr]
x_train = x_train[:used_samples_nr]

nr_of_classes = 20
# tuple (degree, error_value)
error_values = []

for INDEX in range(1,5):
  variable = 0.3 * INDEX

  # ONE TO MANY: (20 classes)
  # class_models[i] - the ith class model
  class_models = []
  avg_sv = 0.0

  # create a model for each class:
  for class_index in range(1, nr_of_classes + 1):
    # change y_train to a ONE to MANY data set
    class_y_train = get_one_to_many_y(y_train, class_index)

    # train a model to recognize class_index from data set:
    prob = svm_problem(class_y_train, x_train)
    param_str = '-s 1 -t 3 -c 0.03125 -b 1 -n 0.05 -q -g 0.0001 -r %f' % (variable)
    param = svm_parameter(param_str)
    model = svm_train(prob, param)
    avg_sv = model.get_nr_sv()
    class_models.append(ClassModel(class_index, model, class_y_train, x_train))
    print("Trained for class %d" % class_index)

  avg_sv = avg_sv / nr_of_classes;
  print("FINISHED Training Models")

  # TEST models:
  test_size = int(used_samples_nr * 0.3)

  for class_model in class_models:
    # read testing data:
    y_test, x_test = svm_read_problem('news20.t.scale')
    y_all_test = y_test[:test_size]
    x_test = x_test[:test_size]
    y_test = get_one_to_many_y(y_all_test, class_model.class_id)
    

    # test on train data:
    p_label_train, p_acc_train, p_val_train = svm_predict(
      class_model.y_train,
      class_model.x_train,
      class_model.model,
      '-b 1'
    )

    # save testing on train data predictions
    class_model.train_predictions = p_label_train
    class_model.train_prob = p_val_train
    

    # test on test data:
    p_label_test, p_acc_test, p_val_test = svm_predict(
      y_test,
      x_test,
      class_model.model,
      '-b 1'
    )

    # save test predictions and probabilities:
    class_model.test_predictions = p_label_test
    class_model.test_prob = p_val_test

  print("FINISHED testing models:")

  save_folder = "sigmoid/coef/"

  # choose from models the highest prob. prediction for each test entry 
  test_predictions = combine_test_predictions(len(y_test), class_models)
  # assemble confusion matrix for testing set:
  cm = plot_confusion_matrix(y_all_test, test_predictions, classes=range(1,21),
                          title=param_str + ' - testare')
  # calculate acc. on testing data:
  hits = 0.0
  for i in range(nr_of_classes):
    hits += cm[i][i]
  acc = hits / test_size
  print("TOTAL TESTACC.: %f" % acc)

  text = "acc: " + str(acc) + "\nerror: " + str(1 - acc) + "\nAvg sv:" + str(avg_sv)
  plt.gcf().text(0, 0, text, ha='left', wrap=True)
  error_values.append((variable, 1 - acc))
  plt.savefig(save_folder + param_str + "_acc=" + str(acc) + '_test.png')



  # assemble confussion matrix for training set y_train:
  train_predictions = combine_train_predictions(len(y_train), class_models)
  cm = plot_confusion_matrix(y_train, train_predictions, classes=range(1,21),
                          title=param_str + ' - training')
  # calculate acc. on testing data:
  hits = 0.0
  for i in range(nr_of_classes):
    hits += cm[i][i]
  acc = hits / len(train_predictions)
  print("TOTAL TRAINACC.: %f" % acc)

  text = "acc: " + str(acc) + "\nerror: " + str(1 - acc) + "\nAvg sv:" + str(avg_sv)
  plt.gcf().text(0, 0, text, ha='left', wrap=True)
  plt.savefig(save_folder + param_str + "_acc=" +str(acc) + '_train.png')


# WRITE ERRORS for plotting:
with open(save_folder + 'errors.txt', 'w') as f:
    for item in error_values:
        f.write("%f %f\n" % (item[0] ,item[1]))


