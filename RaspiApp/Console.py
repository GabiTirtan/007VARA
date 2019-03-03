import inspect


class KeyboardInput:
    '''
        KeyboardInput converter and verifier class
    '''

    @staticmethod
    def get(message="Get something", typeArg=None):
        '''
            General static function to read some type from input and convert to the specified type.

            @param message          The printed message.
            @param typeArg          The type of the input argument                 

            Example:
                get(message = "Please give an integer number",typeArg = int )
                get(message = "Please give a complex number",typeArg = complex )
                get(message = "Please give an A object",typeArg = A )
        '''
        re = None
        while re is None:
            try:
                if typeArg is not None:
                    # Reading a string from the input
                    read_input = input(message)
                    # Evalute the string
                    re = eval(read_input)
                    # Verifiy the type of the input
                    if re.__class__ != typeArg:
                        # Try to convert the evaluated value to the reference type
                        re = typeArg(re)
                else:
                    re = input(message)
            except Exception as e:
                re = None
                print("Wrong input format!", e)

        return re

    @staticmethod
    def getIntegerNumber(message="Get an integer number"):
        '''
            Static function to read an integer number from the input

            @param message          The printed message.
        '''
        return KeyboardInput.get(message=message, typeArg=int)

    @staticmethod
    def getRationalNumber(message="Get an rational number"):
        '''
            Static function to read an rational number from the input

            @param message          The printed message.
        '''
        return KeyboardInput.get(message=message, typeArg=float)


class MenuOption:
    '''
        The class implementing a menu options founctionality. It has a title, a callback function, which will be called after the option selection. 
    '''

    def __init__(self, title, callbackFunc):
        '''
            The constructor function.

            @param title                The title of the sub-menu option
            @param callbackFunc         The function, which will be called automaticly. It must be callable.
        '''
        self.title = title
        if not callable(callbackFunc):
            raise TypeError("CallbackFunc input parameter must be callable!")
        self.callbackFunc = callbackFunc

    def __getInputArgs(self):
        '''
            This function read all input for the callback function and genera a dictionary object with the read inputs. The generated dictionary can be used to call the callback function.
        '''
        #  Dictionary with the input parameter for the callback function__getInputArgs
        args_dict = {}
        # Generating the specification of the callback function
        fullargspec = inspect.getfullargspec(self.callbackFunc)
        # Process all input parameter
        for argName in fullargspec.args:
            # Verify the specification of the input parameter
            if argName != 'self' and argName in fullargspec.annotations:
                # From the annotations getting the type of the input parameter
                variableType = fullargspec.annotations[argName]
                # Reading from the keyboard input the input parameter value
                readinput = KeyboardInput.get(message="\t"+argName+"("+str(variableType)+"):",
                                              typeArg=variableType)
                # Adding to the dictionary the input parameter with the value
                args_dict[argName] = readinput
            elif argName != 'self':
                # The type of the input parameter wasn't defined!
                readinput = KeyboardInput.get(message="\t"+argName+":",
                                              typeArg=None)
                args_dict[argName] = readinput
        return args_dict

    def apply(self):
        '''
            This function is called, after the selection of the sub-menu option
        '''
        #  Print the selectect sub-menu title
        print(" ", self.title.upper(), " ")
        # Getting all input parameter for the callback function
        args_dict = self.__getInputArgs()
        # Call the callback function with the read value fron the keyboard
        self.callbackFunc(**args_dict)


'''

class A:
    def __init__(self, a: int, b: int):
        self.a = a
        self.b = b



'''


def goTo(destinationPoint: complex):
    print(destinationPoint)


def parking():
    print("Parking")


def freeRide():
    print("Free ride")


class MenuManager:
    def __init__(self):
        self.menuOpt_dic = {-1: MenuOption("Exit", self.exit),
                            0: MenuOption("Go to a point", goTo),
                            1: MenuOption("Parking", parking),
                            2: MenuOption("Free ride", freeRide)}
        self.isActive = True

    def exit(self):
        self.isActive = False

    def getOption(self):
        menuOpt = None
        while menuOpt is None:
            opt_key = KeyboardInput.getIntegerNumber("Get menu option:")
            if opt_key in self.menuOpt_dic.keys():
                menuOpt = self.menuOpt_dic[opt_key]
                menuOpt.apply()

    def printMainMenu(self):
        for key in self.menuOpt_dic.keys():
            menuOpt = self.menuOpt_dic[key]
            print(key, ")", menuOpt.title)

    def run(self):
        while(self.isActive):
            self.printMainMenu()
            self.getOption()


def main():
    menuManager = MenuManager()
    menuManager.run()


if __name__ == "__main__":
    main()
