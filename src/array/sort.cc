#include "sort.h"

#ifdef __cplusplus
extern "C" {
#endif

    void push2_sequence (stack<double> &stack, double left, double right) {
        stack.push(right);
        stack.push(left);
    }

    double* quicksort (double sortArray[], double leftPart, double rightPart) {
        stack<double> stack;
        push2_sequence(stack, leftPart, rightPart);
        int _lp, _rp, _mp;

        while(!stack.empty()) {
            double left = stack.top(); stack.pop();
            double right = stack.top(); stack.pop();
            _lp = left;
            _rp = right;
            _mp = sortArray[(_lp + _rp) / 2];

            while(_lp < _rp) {
                while(sortArray[_lp] < _mp) _lp++;
                while(sortArray[_rp] > _mp) _rp--;

                if(_lp <= _rp) {
                    double tmp = sortArray[_lp];
                    sortArray[_lp] = sortArray[_rp];
                    sortArray[_rp] = tmp;
                    _lp++;
                    _rp--;
                }
            }

            if(_lp < right) {
                push2_sequence(stack, _lp, right);
            } 
            
            if(_rp > left) {
                push2_sequence(stack, left, _rp);
            } 
        }

        return sortArray;
    }

    double* EMSCRIPTEN_KEEPALIVE num_sort (double array[], unsigned long length) {
        return quicksort (array, 0, length - 1);
    }


#ifdef __cplusplus
}
#endif
