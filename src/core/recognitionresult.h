#ifndef RECOGNITIONRESULT_H
#define RECOGNITIONRESULT_H

#include <QString>

struct RecognitionResult {
    QString personName;
    double confidence;
    bool matched;
};

#endif // RECOGNITIONRESULT_H
