# GmxInterface

`GmxInterface`, as the name suggests, is an *interface* class, intended to allow client code to access information from `GeoModelXML` and build on top of it within the relevant software stack. `GmxInterface` therefore implements a selection of methods to allow access to information which is expected to be needed elsewhere downstream within the client software. These methods in this base classs from `GeoModelXML` are expected to be overridden in a *derived* class by the client, so that they can provide the necessary context-specific handling of the information. The following methods are implemented in the base class:

- *sensorId/splitSensorId*: To format the indices defined in the [`positionindex`](../../readout/#identifying-sensitive-volumes) into the necessary format for the client code to use. Called whenever a sensitive `logvol` has been encountered, and the result of this method should be written to a `GeoIdentifierTag` for the volume. 
- *addSensorType*: Called whenever a new [`sensortype`](../../readout/#making-volumes-sensitive) is encountered. It allows client code to create and store as appropriate its catalog of possible sensor representations.
- *addSensor/addSplitSensor*: Called whenever a [`sensitive` `logvol`](../../readout/#making-volumes-sensitive) is encountered, allowing a representation of a specific sensor to be created and stored by the client code. 
- *addAlignable/addSplitAlignable:* called whenever an [`alignable` volume](../../alignment/#making-volumes-alignable) is encountered, it allows the `GeoFullPhysVol` and `GeoAlignableTransform` pointers to be passed to the client code for use, as well as any additional information such as the alignment `level` of the volume.
- setPublisher: allows a *publisher* to be set, such that the `readout` information will be written to `SQLite` tables by that publisher.
- publish: write the provided information via the above *publisher*

It also provides helpers for accessing the `readout` information, `getParameter` and `getParameters` for retrieving single and vectors of parameters respectively (plus `checkParameter` for testing the existence of a given parameter in the `readout` definition).



